#include "Player.h"
#include "Assets.h"
#include "IWorld.h"
#include"Field.h"
#include <iostream>


// 移動範囲
const float MovingRangeX = 100.0f;
const float MovingRangeY = 70.0f;

const float Velocity = 1;

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :mesh_(Mesh_Poltergeist, Mesh_CarGhost, Animation_CarGhost){
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 0.75f };
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
    // 移動量を計算
    inputVelocity.normalize();
    float speed = 0.02f;    // 移動スピード
    velocity_ += inputVelocity *delta_time* speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
    if (gsGetKeyTrigger(GKEY_SPACE) == GS_TRUE&& on_ground_) {
        velocity_.y = 0.3f;
        on_ground_ = false;
      //  std::cout << "Jump" << std::endl;
    }
    // 座標を取得
    GSvector3 position = transform_.position();
    // 座標を更新
    position += velocity_;
    // 画面外に出ないように移動範囲を制限する
    position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // 座標の設定
    transform_.position(position);

   
    GSvector3 intersect;//地面との交点
	BoundingSphere sphere = collider();
    if (world_->field()->collide(sphere, &intersect)) {
        //交差した点からy座標のみ補正する
    }
    if (world_->field()->collide(Line{ sphere .center,sphere.center+GSvector2{0,-sphere.radius}}, &intersect)) {
        //交差した点からy座標のみ補正する
        position.y = intersect.y+ sphere.radius;
        on_ground_ = true;
        transform_.position(position);
    }

    //モーション変更
    mesh_.change_motion(motion_);
    //メッシュの更新
    mesh_.update(delta_time);
    //行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}


// 描画
void Player::draw() const {
    glPushMatrix();
    mesh_.draw();
    glMultMatrixf(transform_.localToWorldMatrix());
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    //collider_.draw();
    glPopAttrib();


    glPopMatrix();
}

// 衝突処理
void Player::react(Actor& other) {
    // 敵と衝突した場合は死亡
    if (other.tag() == "EnemyTag" || other.tag() == "EnemyBulletTag") {
        //die();
    }
}