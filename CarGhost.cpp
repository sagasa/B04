#include"CarGhost.h"
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
//�ړ�����̋�����
const float AttackDistance_y{ 50.0f };
//�U������p�x
const float TurnAngle{ 2.5f };
//�G�l�~�[�̍���
const float EnemyHeight{ 90.0f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 45.0f };

//�R���X�g���N�^
CarGhost::CarGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_CarGhost,Mesh_CarGhost,Mesh_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	state_{ State::Idle } {
	world_ = world;
	name_ = "CarGhost";
	tag_ = "EnemyTag";
	player_ = world_->find_actor("Player");
	//transform_.position(GSvector3::zero());
	mesh_.transform(transform_.localToWorldMatrix());
	collider_ = BoundingSphere{ EnemyRadius ,mesh_.bone_matrices(3).position()};
	transform_.position(position);
	transform_.localScale(GSvector3{0.3f,0.3f,0.3f});
	
}

//�X�V
void CarGhost::update(float delta_time) {
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
void CarGhost::draw() const {
	mesh_.draw();
	collider().draw();
}

//�Փ˃��A�N�V����
void CarGhost::react(Actor& other) {
	if (state_ == State::Damage)return;
	if (other.tag() == "PlayerTag") {
		change_state(State::Damage, MotionDamage);
	}
}

//��Ԃ̍X�V
void CarGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Patrol: patrol(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//��ԃ^�C�}�[�X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void CarGhost::change_state(State state, GSuint motion) {
	//���[�V�����ύX
	motion_ = motion;
	//��ԑJ��
	state_ = state;
	//��ԃ^�C�}�[��������
	state_timer_ = 0.0f;
}

//�A�C�h��
void CarGhost::idle(float delta_time) {
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
	//�U�������?
	if (is_turn()) {
		//���Ɍ������H�E�Ɍ������H
		GSuint motion = (target_signed_angle() >= 0.0f) ? MotionIdle : MotionIdle;
		//�U�������ԂɑJ��
		change_state(State::Turn, motion);
		return;
	}
}

//����
void CarGhost::patrol(float delta_time) {
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
void CarGhost::move(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	velocity_ = GSvector3{ to_target().x,to_target().y,0.0f };
	//�X�s�[�h���グ��
	speed_ = 0.25f;
	//�^�[�Q�b�g�����̊p�x�����߂�
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	//�^�[�Q�b�g����������
	transform_.rotate(0.0f, angle, 0.0f);
	//�ړ�
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
	
}

//�^�[��
void CarGhost::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//�U��������[�V�������I��������A�C�h�����ɑJ��
		//idle(delta_time);
	}
	else {
		//�U��������[�V���������Ȃ���^�[�Q�b�g����������
		float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.0f, angle, 0.0f);
	}
}

//�U��
void CarGhost::attack(float delta_time) {
	//���[�V�������I��������Move��ԂɑJ��
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
}

//�_���[�W
void CarGhost::damage(float delta_time) {
	//���[�V�������I��������_���[�W�v�Z
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
void CarGhost::died(float delta_time) {
	//���[�V�������I�������玀�S
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//�U���������
bool CarGhost::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() <= 20.0f);
}

//�U������
bool CarGhost::is_attack()const {
	//�U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����180.0�x�ȉ����H
	return (target_distance() <= AttackDistance) && (target_angle() <= 180.0f);
}

//�ړ�����
bool CarGhost::is_move()const {
	//�ړ��������O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����180.0�x�ȉ����H
	return (target_distance_y() <= AttackDistance_y) && (target_angle() <= 180.0f);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float CarGhost::target_signed_angle()const {
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//�O���������̃x�N�g�����擾
	GSvector3 forward = transform_.forward();
	//
	return GSvector3::signed_angle(forward, to_target);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����Ȃ�)
float CarGhost::target_angle()const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float CarGhost::target_distance()const {
	return (player_->transform().position() - transform_.position()).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 CarGhost::to_target() const {
	return (player_->transform().position() - transform_.position()).normalized();
}

//�^�[�Q�b�g�Ƃ�x�̋��������߂�
float CarGhost::target_distance_x() const {
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�Ƃ�y�̋��������߂�
float CarGhost::target_distance_y() const {
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}