#include"Poltergeist.h"
#include"IWorld.h"
#include"Assets.h"



Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "Poltergeist";
	tag_ = "EnemyTag";
	transform_.position(position);
}

//�X�V
void Poltergeist::update(float delta_time) {
	update_state(delta_time);
}

//�`��
void Poltergeist::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glScalef(30.0f, 30.0f, 30.0f);
	gsDrawMesh(Mesh_Poltergeist);
	glPopMatrix();
}

//�Փ˃��A�N�V����
void Poltergeist::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		//�_���[�W��ԂɕύX
	}
}

//��Ԃ̍X�V
void Poltergeist::update_state(float delta_time) {
	switch (state_) {
	case State::Move: move(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//��ԃ^�C�}�[�X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Poltergeist::change_state(State state, GSuint motion) {
	state_ = state;
	state_timer_ = 0.0f;
}

//�ړ�
void Poltergeist::move(float delta_time) {

}

//�U��
void Poltergeist::attack(float delta_time) {

}

//�_���[�W
void Poltergeist::damage(float delta_time) {
	hp_ -= 1.0f;
	if (hp_ <= 0) {
		//���S��ԂɕύX
	}
}

//���S
void Poltergeist::died(float delta_time) {
	die();
}