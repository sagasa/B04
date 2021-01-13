#include"Poltergeist.h"
#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"DamageProp.h"
#include"Camera.h"
#include<iostream>

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4
};

//�U���������̋���
const float TurnDistance{ 10.0f };
//�ړ�����̋�����
const float MoveDistance{ 10.0f };
//�U������p�x
const float TurnAngle{ 5.0f };
//�U������p�x
const float AttackAngle{ 90.0f };
//�G�l�~�[�̍���
const float EnemyHeight{ 0.75f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 0.5f };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//�ˌ����Ԃ̃C���^�[�o��
const float Interval{240.0f};
//x���W�̎��S���W
const float LimitDistance_x{ 100.0f };
//y���W�̎��S���W
const float LimitDistance_y{ 100.0f };
//�U���͂̐ݒ�
const float atk_power = 1.0f;

//�R���X�g���N�^
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_Poltergeist,Skeleton_Poltergeist,Animation_Poltergeist,MotionIdle},
	motion_{MotionIdle} ,
	motion_loop_{true},
	state_{ State::Idle },
	state_timer_{0.0f},
	player_{nullptr},
	shootiong_timer_{ 0.0f }{
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "Poltergeist";
	//�^�O���̐ݒ�
	tag_ = "EnemyTag";
	//�̗͂̐ݒ�
	hp_ = 1.0f;
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	mesh_.transform(transform_.localToWorldMatrix());
}

//�X�V
void Poltergeist::update(float delta_time) {
	if (transform_.position().x <= 0.0f) {
		change_state(State::Died, MotionDie, false);
	}
	//�J�����̊O���ɂ���Ɖ������Ȃ�
	if (is_inside()) {
		//�v���C���[������
		player_ = world_->find_actor("Player");
		if (player_ == nullptr) {
			player_ = world_->find_actor("PlayerPaladin");
			if (player_ == nullptr) return;
		}
		//��Ԃ̍X�V
		update_state(delta_time);
		//�t�B�[���h�Ƃ̏Փ˔���
		collide_field();
		//���[�V�����ύX
		mesh_.change_motion(motion_);
		//���b�V�����X�V
		mesh_.update(delta_time);
		//�s���ݒ�
		mesh_.transform(transform_.localToWorldMatrix());
		//�ˌ��^�C�}�[�X�V
		shootiong_timer_ -= delta_time;
	}
}

//�`��
void Poltergeist::draw() const {
	mesh_.draw();
	collider().draw();
}

//�Փ˃��A�N�V����
void Poltergeist::react(Actor& other) {
	//�_���[�W���܂��͎��S���̏ꍇ�͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return;

}

bool Poltergeist::on_hit(const Actor& other, float atk_power) {
	//�_���[�W���܂��͎��S���̏ꍇ�͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return false;

	if (other.tag() == "PlayerAttack") {
		hp_ -= atk_power;
		if (hp_ <= 0) {
			//�_���[�W��ԂɕύX
			change_state(State::Damage, MotionDamage, false);
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
void Poltergeist::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Found: found(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//��ԃ^�C�}�[�X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Poltergeist::change_state(State state, GSuint motion,bool loop) {
	//���[�V�����ύX
	motion_ = motion;
	//���[�V�����̃��[�v�w��
	motion_loop_ = loop;
	//��Ԃ̕ύX
	state_ = state;
	//��ԃ^�C�}�[�̏�����
	state_timer_ = 0.0f;
}

//�A�C�h��
void Poltergeist::idle(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Found, MotionIdle);
		return;
	}
	//�����Ȃ���΃A�C�h����
	change_state(State::Idle, MotionIdle);
}

//�^�[��
void Poltergeist::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//�U��������[�V�������I�������甭�����ɑJ��
		found(delta_time);
	}
	else {
		//�U��������[�V���������Ȃ���^�[�Q�b�g����������
		float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.0f, angle, 0.0f);
	}
}

//�v���C���[��������
void Poltergeist::found(float delta_time) {
	//�U�������?
	if (is_turn()) {
		change_state(State::Turn, MotionIdle);
		return;
	}
	//�U�����邩�H
	if (is_attack() && shootiong_timer_ <= 0.0f) {
		change_state(State::Attack, MotionAttack);
		return;
	}
}

//�U��
void Poltergeist::attack(float delta_time) {
	//���[�V�����I����Ɉړ����ɑJ��
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
	else if (state_timer_ >= mesh_.motion_end_time() - 30.0f && shootiong_timer_ <= 0.0f) {
		generate_bullet();
		shootiong_timer_ = Interval;
	}
}

//�_���[�W
void Poltergeist::damage(float delta_time) {
	//���[�V�����I����ɃA�C�h����
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
	
}

//���S
void Poltergeist::died(float delta_time) {
	//���[�V�����I����Ɏ��S����
	if (state_timer_ >= mesh_.motion_end_time()-30.0f) {
		die();
	}
	
}

//�U���������
bool Poltergeist::is_turn()const {
	return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

//�U������
bool Poltergeist::is_attack()const {
	//�U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance_x() <= MoveDistance) && (target_angle() <= AttackAngle);
}

//�J�����̓����ɂ��邩�H
bool Poltergeist::is_inside() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//��ʓ��ɂ�����ړ�����
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//�J�����̑O�x�N�g��
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle <= 45.0f);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float Poltergeist::target_signed_angle()const {
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
float Poltergeist::target_angle()const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float Poltergeist::target_distance()const {
	//�^�[�Q�b�g�����Ȃ���΍ő�l��Ԃ�
	if (player_ == nullptr) return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//�^�[�Q�b�g�Ƃ�x�̋��������߂�
float Poltergeist::target_distance_x() const {
	//�^�[�Q�b�g�����Ȃ���΍ő�l��Ԃ�
	if (player_ == nullptr) return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�Ƃ�y�̋��������߂�
float Poltergeist::target_distance_y() const {
	//�^�[�Q�b�g�����Ȃ���΍ő�l��Ԃ�
	if (player_ == nullptr) return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 Poltergeist::to_target() const {
	//�^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (player_ == nullptr)return GSvector3::zero();
	return (player_->transform().position() - transform_.position()).normalized();
}

//�t�B�[���h�Ƃ̏Փˏ���
void Poltergeist::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	BoundingSphere sphere{ collider_.radius,transform_.position() };
	GSvector3 center;//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		//y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		//�␳��̍��W�ɕύX����
		transform_.position();
	}
}

//�A�N�^�[�Ƃ̏Փˏ���
void Poltergeist::collide_actor(Actor& other) {
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
	v.z = 0.0f;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}

//�e�̐���
void Poltergeist::generate_bullet() const {
	//�e�𐶐�����ꏊ�̋���
	const float GenerateDistance{ 0.75f };
	//�e�̐����ʒu�̍����̕␳�l
	const float GenerateHeight{ 1.0f };
	//�e�̃X�s�[�h
	const float BulletSpeed{ 0.075f };
	//�����ʒu�̌v�Z
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	//y���W��␳����
	position.y += GenerateHeight;
	//z���W��0�ɂ���
	position.z = 0.0f;
	//�ړ��ʂ̌v�Z
	GSvector3 velocity = to_target() * BulletSpeed;
	//�e�̐���
	world_->add_actor(new PoltergeistBullet{ world_,position,velocity, atk_power});
}