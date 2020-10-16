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
    // �ړ��ʂ��v�Z
    float speed = 1.0f;    // �ړ��X�s�[�h
    velocity_ = inputVelocity.normalized() * speed * delta_time;
    // ���W���擾
    GSvector3 position = transform_.position();
    // ���W�Ɉړ��ʂ����Z
    position += velocity_;
    // ���W�̐ݒ�
    transform_.position(position);
}
void SurogSakones::draw()const{
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
    collider().draw();
	glPopMatrix();
}
