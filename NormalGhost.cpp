#include"NormalGhost.h"
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
};

//�U���������̋���
const float TurnDistance{ 1.5f };
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
const float Speed{ 0.025f };

//�R���X�g���N�^
NormalGhost::NormalGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_CarGhost,Skeleton_CarGhost,Animation_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	player_{ nullptr },
	hp_{ 1 } {
		//���[���h�̐ݒ�
		world_ = world;
		//���O�̐ݒ�
		name_ = "NormalGhost";
		//�^�O��
		tag_ = "EnemyTag";
		collider_ = BoundingSphere{ EnemyRadius,GSvector3{0.0f,EnemyHeight,0.0f} };
		//���W�̏�����
		transform_.position(position);
		transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
		//���b�V���̕ϊ��s���������
		mesh_.transform(transform_.localToWorldMatrix());
}

//�X�V
void NormalGhost::update(float delta_time) {
	player_ = world_->find_actor("Player");
}

//�`��
void NormalGhost::draw() const {

}

//�Փ˃��A�N�V����
void NormalGhost::react(Actor& other) {

}

//��Ԃ̍X�V
void NormalGhost::update_state(float delta_time) {

}

//��Ԃ̕ύX
void NormalGhost::change_state(State state, GSuint motion, bool loop) {

}

//�A�C�h����
void NormalGhost::idle(float delta_time) {

}

//�ړ���
void NormalGhost::move(float delta_time) {

}

//�_���[�W��
void NormalGhost::damage(float delta_time) {

}

//���S��
void NormalGhost::died(float delta_time) {

}

//�^�[�Q�b�g�����̊p�x�����߂�(�����t)
float NormalGhost::target_signed_angle() const {
	if (player_ == nullptr) return 0.0f;
	GSvector3 to_target = player_->transform().position() - transform_.position();
	GSvector3 forward = transform_.forward();
	return GSvector3::signed_angle(forward,to_target);
}

//�^�[�Q�b�g�����̊p�x�����߂�
float NormalGhost::target_angle() const {
	return std::abs(target_signed_angle());
}

//�^�[�Q�b�g�Ƃ̋��������߂�
float NormalGhost::target_distance() const {
	if (player_ == nullptr)return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 NormalGhost::to_target() const {
	if (player_ == nullptr) return GSvector3::zero();
	return (player_->transform().position() - transform_.position()).normalized();
}