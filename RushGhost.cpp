#include"RushGhost.h"
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
const float AttackDistance{ 15.0f };
//�ړ�����̋���x
const float AttackDistance_x{ 100.0f };
//�ړ�����̋�����
const float AttackDistance_y{ 50.0f };
//�U������p�x
const float TurnAngle{ 2.5f };
//�G�l�~�[�̍���
const float EnemyHeight{ 90.0f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 45.0f };

//�R���X�g���N�^
RushGhost::RushGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_RushGhost,Mesh_CarGhost,Mesh_CarGhost, MotionIdle },
	motion_{ MotionIdle },
	state_{ State::Idle } {
	world_ = world;
	name_ = "RushGhost";
	tag_ = "EnemyTag";
	player_ = world_->find_actor("Player");
	mesh_.transform(transform_.localToWorldMatrix());
	collider_ = BoundingSphere{ EnemyRadius ,mesh_.bone_matrices(3).position() };
	transform_.position(position);
	transform_.localScale(GSvector3{ 0.3f,0.3f,0.3f });

}

//�X�V
void RushGhost::update(float delta_time) {
	//��Ԃ̍X�V
	update_state(delta_time);
	//���[�V�����ύX
	mesh_.change_motion(motion_);
	//���b�V���̍X�V
	mesh_.update(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
}

//�`��
void RushGhost::draw() const {
	mesh_.draw();
	collider().draw();
}

//�Փ˃��A�N�V����
void RushGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		change_state(State::Damage, MotionDamage);
	}
}

//��Ԃ̍X�V
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Patrol: patrol(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//��ԃ^�C�}�[�̍X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void RushGhost::change_state(State state, GSuint motion) {
	//���[�V�����ύX
	motion_ = motion;
	//��Ԃ̕ύX
	state_ = state;
	//��ԃ^�C�}�[������
	state_timer_ = 0.0f;
}

//�A�C�h��
void RushGhost::idle(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	//�v���C���[�����������H
	if (is_move()) {
		change_state(State::Move, MotionIdle);
		return;
	}
}

//����
void RushGhost::patrol(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	//�v���C���[�����������H
	if (is_move()) {
		change_state(State::Move, MotionIdle);
		return;
	}
}

//�ړ�
void RushGhost::move(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
	}
	velocity_ = GSvector3{ to_target().x,to_target().y,0.0f };
	//�X�s�[�h���グ��
	speed_ = 0.5f;
	//�^�[�Q�b�g�����̊p�x�����߂�
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	//�^�[�Q�b�g����������
	transform_.rotate(0.0f, angle, 0.0f);
	//transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//�^�[��
void RushGhost::turn(float delta_time) {

}

//�U��
void RushGhost::attack(float delta_time) {
	//���[�V�����I����Ɉړ����ɑJ��
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
}

//�_���[�W
void RushGhost::damage(float delta_time) {
	//���[�V�����I����Ƀ_���[�W�v�Z
	if (state_timer_ >= mesh_.motion_end_time()) {
		//�v���C���[��atk������
		hp_ -= 1.0f;
		if (hp_ <= 0) {
			//Die�ɏ�ԕύX
			change_state(State::Died, MotionDie);
		}
	}

}

//���S
void RushGhost::died(float delta_time) {
	//���[�V�����I����Ɏ��S
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}

}

//�U���������
bool RushGhost::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() <= 20.0f);
}

//�U������
bool RushGhost::is_attack()const {
	//�U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance() <= AttackDistance) && (target_angle() <= 180.0f);
}

//�ړ�����
bool RushGhost::is_move()const {
	//�ړ��������O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����100.0�x�ȉ����H
	return (target_distance_x() <= AttackDistance_x) && (target_distance_y() <= AttackDistance_y) && (target_angle() <= 180.0f);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float RushGhost::target_signed_angle()const {
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//�O���������̃x�N�g�����擾
	GSvector3 forward = transform_.forward();
	//
	return GSvector3::signed_angle(forward, to_target);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����Ȃ�)
float RushGhost::target_angle()const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float RushGhost::target_distance()const {
	return (player_->transform().position() - transform_.position()).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 RushGhost::to_target() const {
	return (player_->transform().position() - transform_.position()).normalized();
}

//�^�[�Q�b�g�Ƃ�x�̋��������߂�
float RushGhost::target_distance_x() const {
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�Ƃ�y�̋��������߂�
float RushGhost::target_distance_y() const {
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}