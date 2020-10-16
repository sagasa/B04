#include "SurogSakones.h"
#include"IWorld.h";
#include"Field.h";

SurogSakones::SurogSakones(IWorld* world,const GSvector3& position){
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "SurogSakones";
	transform_.position(position);
	collider_ = BoundingSphere(5);
    state_ = State::Normal;
    hp_ = 100.0f;
}
void SurogSakones::update(float delta_time){
    if (hp_ <= 0)die();
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
    float speed = 1.0f;    // 移動スピード
    velocity_ = inputVelocity.normalized() * speed * delta_time;
    // 座標を取得
    GSvector3 position = transform_.position();
    // 座標に移動量を加算
    position += velocity_;
    // 座標の設定
    transform_.position(position);
}
void SurogSakones::draw()const{
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
    collider().draw();
	glPopMatrix();
}
