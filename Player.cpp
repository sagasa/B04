#include "Player.h"
#include "Assets.h"
#include "IWorld.h"
#include"Field.h"
#include <iostream>


#include "player_ghost.h"
#include "player_paladin.h"


// 移動範囲
const float MovingRangeX = 100.0f;
const float MovingRangeY = 70.0f;

const float Velocity = 0.15f;

static player_paladin Paladin =player_paladin{};
static player_ghost Ghost = player_ghost{};

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :info(Paladin){//どうすりゃいいのヨ
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 0.8f,GSvector3{0.0f,0.8f,0.0f} };
    transform_.localRotation(GSquaternion::euler( 0.0f, 90.0f, 0.0f ));
}

const GSvector3 gravity{ 0.0f, 0.02f, 0.0f };
// 更新
void Player::update(float delta_time) {
    //重力？
    velocity_ -= gravity * delta_time;
    velocity_ *= 0.9f;

    // キーボードの入力から移動量を決める
    GSvector3 inputVelocity{ 0.0f, 0.0f, 0.0f };
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
    if (inputVelocity.x < 0) {
        transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
    }
    else if(inputVelocity.x > 0){
        transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
    }
    // 移動量を計算
    inputVelocity.normalize();
    float speed = 0.04f;    // 移動スピード
    velocity_ += inputVelocity *delta_time* speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
    if (gsGetKeyTrigger(GKEY_SPACE) == GS_TRUE&& on_ground_) {
        jump_count_ = 5;
        velocity_.y = MIN(0, velocity_.y);
        on_ground_ = false;
      //  std::cout << "Jump" << std::endl;
    }
	if(0<jump_count_)
	{
        jump_count_--;
        velocity_.y += 0.1f;
	}
    // 座標を取得
    //GSvector3 position = transform_.position();
    // 座標を更新
    //position += velocity_;
    // 画面外に出ないように移動範囲を制限する
    //position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    //position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // 座標の設定
    //transform_.position(position);
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);


   
    /*GSvector3 intersect;//地面との交点
	BoundingSphere sphere = collider();
    if (world_->field()->collide(sphere, &intersect)) {
        //交差した点からy座標のみ補正する
        intersect.y = position.y;
        transform_.position(intersect);
    }
    if (world_->field()->collide(Line{ position + sphere .center,sphere.center+GSvector2{0,-sphere.radius}}, &intersect)) {
        //交差した点からy座標のみ補正する
        position.y = intersect.y;
        on_ground_ = true;
        transform_.position(position);
        velocity_.y = 0.0f;
    }*/

   
    {
        //地面との衝突判定(線分との交差判定)
        BoundingSphere sphere = collider();
        if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,-sphere.radius} })) {
			//ジャンプ可能に
            on_ground_ = true;
        }
        if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,sphere.radius} })) {
            //ジャンプ停止
            jump_count_ = 0;
        }
    }
    
    //壁との衝突判定(球体との判定)
    {
        BoundingSphere sphere = collider();
        GSvector3 center;
        //衝突後の球体の中心座標
        if (world_->field()->collide(sphere, &center)) {
            

            //座標系を戻す
            center-= collider_.center;
            GSvector3 vel = (center - transform_.position());
    	
            velocity_+= vel*delta_time*0.2f;
            velocity_.z = 0;

        	//補正後の座標に変更する
           // transform_.position(center);
        }
    }
    
    info.update(static_cast<Actor&>(*this),delta_time);
   
}


// 描画
void Player::draw() const {
    info.draw();
	glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();


    glPopMatrix();
}

// 衝突処理
void Player::react(Actor& other) {
    // 敵と衝突した場合は死亡
    if (other.tag() == "EnemyTag" || other.tag() == "EnemyBulletTag") {
        info.hit();
    }
}