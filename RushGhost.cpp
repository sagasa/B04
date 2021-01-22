#include"RushGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"DamageProp.h"
#include"ParticleManager.h"

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4,
	MotionRun = 5
};

//�U���������̋���
const float TurnDistance{ 1.5f };
//�ړ�����̋���
const float MoveDistance{ 7.0f };
//�U������p�x
const float TurnAngle{ 5.0f };
//�G�l�~�[�̍���
const float EnemyHeight{ 0.75f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 0.5f };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//�~����
const float PI{ 3.141592654 };
//���a
const float radius{ 5.0f };
//x���W�̎��S���W
const float LimitDistance_x{ 100.0f };
//y���W�̎��S���W
const float LimitDistance_y{ 100.0f };
//�U���͂̐ݒ�
const float atk_power = 1.0f;
//�ړ����ɑJ�ڂ���܂ł̃C���^�[�o��
const float Interval{ 60.0f };

//�R���X�g���N�^
RushGhost::RushGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_RushGhost,Skeleton_RushGhost,Animation_RushGhost, MotionIdle },
	motion_{ MotionIdle },
	state_{ State::Idle },
	angle_{0.0f},
	point_{0.0f} {
	world_ = world;
	name_ = "RushGhost";
	tag_ = "EnemyTag";
	//�̗͂̐ݒ�
	hp_ = 3.0f;
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	//���W�̏�����
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	//���b�V���̕ϊ��s��̏�����
	mesh_.transform(transform_.localToWorldMatrix());

}

//�X�V
void RushGhost::update(float delta_time) {
	//�v���C���[������
	player_ = world_->find_actor("Player");
	if (player_ == nullptr) {
		player_ = world_->find_actor("PlayerPaladin");
		if (player_ == nullptr) return;
	}
	//��Ԃ̍X�V
	update_state(delta_time);
	//�t�B�[���h�Ƃ̏Փ˔���
	//collide_field();
	//���[�V�����ύX
	mesh_.change_motion(motion_);
	//���b�V���̍X�V
	mesh_.update(delta_time);
	//�v���C���[�̕����֌���
	turn(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
}

//�`��
void RushGhost::draw() const {
	mesh_.draw();
	collider().draw();
	if (state_ == State::Died) {
		world_->particle_manager()->death_smoke(transform_.position());
	}
}

//�Փ˃��A�N�V����
void RushGhost::react(Actor& other) {
	//�_���[�W���܂��͎��S���̏ꍇ�͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return;

	if (other.tag() == "PlayerTag") {
		DamageProp::do_attack(other, *this, atk_power);
	}
}


bool RushGhost::on_hit(const Actor& other, float atk_power) {
	//�_���[�W���܂��͎��S���̏ꍇ�͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return false;
	if (other.tag() == "PlayerAttack") {
		hp_ -= atk_power;
		if (hp_ <= 0) {
			//�_���[�W��ԂɕύX
			change_state(State::Died, MotionDie, false);
		}
		else {
			//�U���̐i�s�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//�_���[�W��ԂɕύX
			change_state(State::Damage, MotionDamage, false);
		}
		return true;
	}
	return false;
}

//��Ԃ̍X�V
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//��ԃ^�C�}�[�̍X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void RushGhost::change_state(State state, GSuint motion,bool loop) {
	//���[�V�����ύX
	motion_ = motion;
	//���[�V�����̃��[�v�w��
	motion_loop_ = loop;
	//��Ԃ̕ύX
	state_ = state;
	//��ԃ^�C�}�[������
	state_timer_ = 0.0f;
}

//�A�C�h��
void RushGhost::idle(float delta_time) {
	//�v���C���[�������Ă��ړ��^�C�}�[��0�ɂȂ��Ă��邩�H
	if (is_move() && moving_timer_ <= 0.0f) {
		change_state(State::Move, MotionIdle);
		moving_timer_ = Interval;
		point_ = 0;
		angle_ = 0.0f;
		to_targe_angle_ = (transform_.position().x < player_->transform().position().x) ? -1 : 1;
		rotate_center_ = transform_.position() - to_targe_angle_* GSvector3{ radius,0.0f,0.0f};
		return;
	}
	//�ړ��^�C�}�[���X�V
	moving_timer_ -= delta_time;
	change_state(State::Idle, MotionIdle);
}

//�ړ�
void RushGhost::move(float delta_time) {
	//�}�b�v�O�ɂ����玀�S
	if (transform_.position().x <= -10.0f) {
		change_state(State::Died, MotionDie, false);
		return;
	}
	//��
	const int NumOfTimes{ 90};//�ړ����x���ω�
	if (point_ <= NumOfTimes) {//��
		angle_ = PI * point_ / NumOfTimes;
		GSvector3 position{  rotate_center_.x + to_targe_angle_ *(float)cos(angle_) * radius,rotate_center_.y + -(float)sin(angle_) * radius,0.0f };
		GSvector3 velocity = (position - transform_.position());
		velocity_ = velocity;
		transform_.translate(velocity_, GStransform::Space::World);
		point_ += delta_time;
	}
	else {
		transform_.position(rotate_center_ - to_targe_angle_ * GSvector3{ radius,0.0f,0.0f });
		change_state(State::Idle, MotionIdle);
	}
}

//�^�[��
void RushGhost::turn(float delta_time) {
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	//�^�[�Q�b�g����������
	transform_.rotate(0.0f, angle, 0.0f);
}

//�_���[�W
void RushGhost::damage(float delta_time) {
	//���[�V�����I����Ɉړ�����
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
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
	return(target_distance() <= TurnDistance) && (target_angle() >= 20.0f);
}

//�ړ�����
bool RushGhost::is_move()const {
	//�ړ�����?
	return (target_distance() <= MoveDistance);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float RushGhost::target_signed_angle()const {
	//�^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (player_ == nullptr)return 0.0f;
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
	if (player_ == nullptr)return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 RushGhost::to_target() const {
	//�^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (player_ == nullptr)return GSvector3::zero();
	return (player_->transform().position() - transform_.position()).normalized();
}

//�^�[�Q�b�g�Ƃ�x�̋��������߂�
float RushGhost::target_distance_x() const {
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�Ƃ�y�̋��������߂�
float RushGhost::target_distance_y() const {
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//�t�B�[���h�Ƃ̏Փˏ���
void RushGhost::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	BoundingSphere sphere{ collider().radius,transform().position() };
	GSvector3 center;//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(sphere, &center)) {
		center.z = 0.0f;
		//�␳��̍��W�ɕύX����
		transform_.position(center);
	}
}

//�A�N�^�[�Ƃ̏Փˏ���
void RushGhost::collide_actor(Actor& other) {
	//z���W���������W�����߂�
	GSvector3 position = transform_.position();
	position.z = 0.0f;
	GSvector3 target = other.transform().position();
	target.z = 0.0f;
	//����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	//�Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius + other.collider().radius;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}