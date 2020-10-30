#include"Poltergeist.h"
#include"IWorld.h"
#include"Assets.h"

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4
};

//�U���������̋���
const float TurnDistance{ 1.5f };
//�U������̋���
const float AttackDistance{ 1.2f };
//�ړ�����̋���
const float WalkDistance{ 10.0f };
//�U������p�x
const float TurnAngle{ 2.5f };

//�R���X�g���N�^
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_CarGhost,Skeleton_CarGhost,Animation_CarGhost,0},
	motion_{0} ,
	state_{ State::Move } {
	world_ = world;
	name_ = "Poltergeist";
	tag_ = "EnemyTag";
	transform_.position(position);
	player_ = world_->find_actor("Player");
}

//�X�V
void Poltergeist::update(float delta_time) {
	//��Ԃ̍X�V
	update_state(delta_time);
	//���[�V�����ύX
	mesh_.change_motion(motion_);
	//���b�V�����X�V
	mesh_.update(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());

}

//�`��
void Poltergeist::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glScalef(0.7f, 0.7f, 0.7f);
	mesh_.draw();
	glPopMatrix();
}

//�Փ˃��A�N�V����
void Poltergeist::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		//�_���[�W��ԂɕύX
		change_state(State::Damage, MotionDamage);
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
	//���[�V�����ύX
	motion_ = motion;
	//��Ԃ̕ύX
	state_ = state;
	//��ԃ^�C�}�[�̏�����
	state_timer_ = 0.0f;
}

//�ړ�
void Poltergeist::move(float delta_time) {

}

//�U��
void Poltergeist::attack(float delta_time) {
	//���[�V�����I����Ɉړ����ɑJ��
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
	}
}

//�_���[�W
void Poltergeist::damage(float delta_time) {
	//���[�V�����I����Ƀ_���[�W�v�Z
	if (state_timer_ >= mesh_.motion_end_time()) {
		hp_ -= 1.0f;
		if (hp_ <= 0) {
			//���S��ԂɕύX
			change_state(State::Died, MotionDie);
		}
	}
	
}

//���S
void Poltergeist::died(float delta_time) {
	//���[�V�����I����Ɏ��S����
	if (state_timer_ >= mesh_.motion_end_time()) {
		die();
	}
	
}

//�U���������
bool Poltergeist::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() <= 20.0f);
}

//�U������
bool Poltergeist::is_attack()const {
	//�U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

//�ړ�����
bool Poltergeist::is_move()const {
	//�ړ��������O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����100.0�x�ȉ����H
	return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float Poltergeist::target_signed_angle()const {
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//�O���������̃x�N�g�����擾
	GSvector3 forward = transform_.forward();
	//
	return GSvector3::signed_angle(forward, to_target);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����Ȃ�)
float Poltergeist::target_angle()const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float Poltergeist::target_distance()const {
	return (player_->transform().position() - transform_.position()).magnitude();
}