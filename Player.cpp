#include "Player.h"
#include "Assets.h"
#include "IWorld.h"


// 移動範囲
const float MovingRangeX = 100.0f;
const float MovingRangeY = 70.0f;

const float Velocity = 1;

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) {
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 5.0f };
}

const GSvector3 gravity{ 0.0f, 1.0f, 0.0f };
// 更新
void Player::update(float delta_time) {
    //重力？
    velocity_ -= gravity.getNormalized() * delta_time;


    // 自機がｙ軸プラス方向を向くように回転させる
    transform_.eulerAngles(-90.0f, 180.0f, 0.0f);
    // キーボードの入力から移動量を決める
    GSvector3 inputVelocity{ 0.0f, 0.0f, 0.0f };
    float velX = 0;
    if (gsGetKeyState(GKEY_LEFT) == GS_TRUE) {
        velX = -1.0f;
    }
    if (gsGetKeyState(GKEY_RIGHT) == GS_TRUE) {
        velX = 1.0f;
    }
    if (gsGetKeyState(GKEY_UP) == GS_TRUE) {
        inputVelocity.y = 1.0f;
    }
    if (gsGetKeyState(GKEY_DOWN) == GS_TRUE) {
        inputVelocity.y = -1.0f;
    }
    // 移動量を計算
    float speed = 1.0f;    // 移動スピード
    //velocity_ = inputVelocity.getNormalized() * speed * delta_time;
    velocity_.x += velX*delta_time;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);

    // 座標を取得
    GSvector3 position = transform_.position();
    // 座標を更新
    position += velocity_;
    // 画面外に出ないように移動範囲を制限する
    position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // 座標の設定

    transform_.position(position);


  
    // Zキーを押したら弾を発射する
    if (gsGetKeyTrigger(GKEY_Z) == GS_TRUE) {
    }
}


// 描画
void Player::draw() const {
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(Mesh_Player);

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
        die();
    }
}