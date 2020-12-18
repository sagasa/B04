#include "Player.h"
#include "IWorld.h"
#include"Field.h"
#include <iostream>
#include "AttackCollider.h"

void Player::update_physics(const float delta_time)
{
    // 座標を更新
    transform_.translate(velocity_.xy * delta_time, GStransform::Space::World);
	
    //地面との衝突判定(線分との交差判定)
    BoundingSphere sphere = collider();
    //ジャンプ可能に
    on_ground_ = world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,-sphere.radius} });
	
    if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,sphere.radius} })) {
        //ジャンプ停止
        jump_count_ = 0;
    }

    //壁との衝突判定(球体との判定)
    GSvector3 center;
    //衝突後の球体の中心座標
    if (world_->field()->collide(sphere, &center)) {
        //座標系を戻す
        center -= collider_.center;
        if (is_soft_)
        {
        	//ゴースト時のふわふわ
            GSvector3 vel = (center - transform_.position());
            velocity_ += vel * delta_time * 0.2f;
            velocity_.z = 0;
        }else
        {
            //押し戻し
            transform_.position(center.xy);
        }
    }

	//ジャンプ更新
    if (0 < jump_count_)
    {
        jump_count_--;
        velocity_.y += jump_force_;
    }
}

void Player::jump(const int count, const float force)
{
	if(on_ground_)
	{
        velocity_.y = MIN(0, velocity_.y);
        jump_count_ = count;
        jump_force_ = force;
	}
}

GSvector2 Player::get_input()
{
    // キーボードの入力から移動量を決める
    GSvector2 inputVelocity{0,0};
    if (gsGetKeyState(GKEY_LEFT) == GS_TRUE) {
        inputVelocity.x = -1.0f;
    }
    if (gsGetKeyState(GKEY_RIGHT) == GS_TRUE) {
        inputVelocity.x = 1.0f;
    }
    if (gsGetKeyState(GKEY_UP) == GS_TRUE) {
        inputVelocity.y = 1.0f;
    }
    if (gsGetKeyState(GKEY_DOWN) == GS_TRUE) {
        inputVelocity.y = -1.0f;
    }
    return inputVelocity.normalized();
}

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position, const AnimatedMesh& mesh) :mesh_{ mesh }{//どうすりゃいいのヨ
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 0.8f,GSvector3{0.0f,0.8f,0.0f} };
    transform_.localRotation(GSquaternion::euler( 0.0f, 90.0f, 0.0f ));
}

// 描画
void Player::draw() const {
    mesh_.draw();
	glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();


    glPopMatrix();
}