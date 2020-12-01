#include"CarGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4,
	MotionRun = 5,
	MotioinSpawn = 6
};

//�U���������̋���
const float TurnDistance{ 5.0f };
//�U������̋���
const float AttackDistance{ 1.5f };
//�ړ�����̋�����
const float MoveDistance_y{ 5.0f };
//�U������p�x
const float TurnAngle{ 5.0f };
//�G�l�~�[�̍���
const float EnemyHeight{ 0.75f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 0.5f };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//����̃I�t�Z�b�g
const float HeadOffset{ 1.0f };
//�X�s�[�h
const float Speed{ 0.05f };

//�R���X�g���N�^
CarGhost::CarGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Poltergeist,Skeleton_CarGhost,Animation_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{true},
	state_{ State::Idle },
	state_timer_{0.0f},
	player_{ nullptr },
	hp_{3},
	moving_timer_{gsRandf(0,60.0f)},
	is_turn_{false} {
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "CarGhost";
	//�^�O���̐ݒ�
	tag_ = "EnemyTag";
	//transform_.position(GSvector3::zero());
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	//���W�̏�����
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	//���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
}

//�X�V
void CarGhost::update(float delta_time) {
	//�v���C���[������
	player_ = world_->find_actor("Player");
	//��Ԃ̍X�V
	update_state(delta_time);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
	//���[�V�����ύX
	mesh_.change_motion(motion_);
	//���b�V���̍X�V
	mesh_.update(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
	//�^�C�}�[�X�V
	moving_timer_ -= delta_time;
}

//�`��
void CarGhost::draw() const {
	mesh_.draw();
	collider().draw();
}

//�Փ˃��A�N�V����
void CarGhost::react(Actor& other) {
	//�_���[�W���܂��͎��S���͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died)return;
	if (other.tag() == "PlayerAttackTag") {
		hp_--;
		if (hp_ <= 0) {
			//�_���[�W��ԂɕύX
			change_state(State::Died, MotionDie,false);
		} else {
			//�U���̐i�s�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//�_���[�W��ԂɕύX
			change_state(State::Damage, MotionDamage,false);
		}
		return;
	}
	//�v���C���[�܂��̓G�l�~�[�ɏՓ˂����ꍇ
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
		change_state(State::Move, MotionRun);
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
void CarGhost::change_state(State state, GSuint motion,bool loop) {
	//���[�V�����ύX
	motion_ = motion;
	//���[�V�����̃��[�v�w��
	motion_loop_ = loop;
	//��ԑJ��
	state_ = state;
	//��ԃ^�C�}�[��������
	state_timer_ = 0.0f;
}

//�A�C�h��
void CarGhost::idle(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack,false);
		return;
	}
	//�v���C���[�����������H
	if (is_move()) {
		change_state(State::Move, MotionRun);
		return;
	}
	if (is_turn()) {
		change_state(State::Turn, MotionIdle);
	}
	//�����Ȃ���Ώ���
	change_state(State::Patrol, MotionIdle);
}

//����
void CarGhost::patrol(float delta_time) {

	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack,false);
	}
	//�v���C���[�����������H
	if (is_move()) {
		change_state(State::Move, MotionRun);
		return;
	}
	if (is_turn()) {
		change_state(State::Turn, MotionIdle);
	}
	//�����Ȃ���Ώ���
	change_state(State::Patrol, MotionIdle);

}

//�ړ�
void CarGhost::move(float delta_time) {
	//�U�����邩�H
	/*if (is_attack()) {
		change_state(State::Attack, MotionAttack, false);
	}*/
	if (is_turn()) {
		change_state(State::Turn, MotionIdle);
	}
	if (moving_timer_ <= 0) {
		velocity_ = GSvector3{ to_target().x,to_target().y,0.0f };
		moving_timer_ = gsRandf(30.0f, 60.0f);
	}
	/*if (player_ ->transform().position().y > transform_.position().y) {
		
	}
	else if (player_->transform().position().y < transform_.position().y) {
		transform_.rotate(GSvector3{ -1.0f,0.0f,0.0f }, 20.0f);
	}
	//�^�[�Q�b�g�����̊p�x�����߂�
		float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
		//�^�[�Q�b�g����������
		transform_.rotate(angle, 0.0f, 0.0f);*/
	//�ړ�
	transform_.translate(velocity_ * delta_time * Speed, GStransform::Space::World);
	
}

//�^�[��
void CarGhost::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//�U��������[�V�������I��������A�C�h�����ɑJ��
		idle(delta_time);
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
	if (state_timer_ < mesh_.motion_end_time()) {
		//�m�b�N�o�b�N����
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
		velocity_ -= GSvector3{ velocity_.x,velocity_.y,0.0f } *0.5f, delta_time;
	}
	//���[�V�������I��������A�C�h����
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}



}

//���S
void CarGhost::died(float delta_time) {
	//���[�V�������I�������玀�S
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//�U������
bool CarGhost::is_attack()const {
	//�U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

//�ړ�����
bool CarGhost::is_move()const {
	//�ړ��������O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance_y() <= MoveDistance_y) && (target_angle() <= 20.0f);
}

//�U���������
bool CarGhost::is_turn() const {
	//�U��������������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20�x�ȉ����H
	return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float CarGhost::target_signed_angle()const {
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
float CarGhost::target_angle()const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float CarGhost::target_distance()const {
	if (player_ == nullptr)return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//�^�[�Q�b�g�Ƃ�x�̋��������߂�
float CarGhost::target_distance_x() const {
	//�^�[�Q�b�g�����Ȃ���΍ő�l��Ԃ�
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�Ƃ�y�̋��������߂�
float CarGhost::target_distance_y() const {
	//�^�[�Q�b�g�����Ȃ���΍ő�l��Ԃ�
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 CarGhost::to_target() const {
	//�^�[�Q�b�g�����Ȃ����0��Ԃ�
	if (player_ == nullptr)return GSvector3::zero();
	GSvector3 player = player_->transform().position() - GSvector3{1.5f,1.5f,0.0f};
	return (player - transform_.position()).normalized();
}

//�t�B�[���h�Ƃ̏Փˏ���
void CarGhost::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	BoundingSphere sphere{ collider().radius,transform().position() };
	GSvector3 center;//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(sphere,&center)) {
		center.z = 0.0f;
		//�␳��̍��W�ɕύX����
		transform_.position(center);
	}
}

//�A�N�^�[�Ƃ̏Փˏ���
void CarGhost::collide_actor(Actor& other) {
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