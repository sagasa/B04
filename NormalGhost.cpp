#include"NormalGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"Camera.h"
#include"DamageProp.h"
#include"ParticleManager.h"
#include"Camera.h"

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
//�U������̋���
const float AttackDistance{ 1.5f };
//�ړ�����̋�����
const float MoveDistance{10.0f};
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
//�ړ����x
const float Speed{ 0.05f };
//x���W�̎��S���W
const float LimitDistance_x{ 100.0f };
//y���W�̎��S���W
const float LimitDistance_y{ 100.0f };

const float atk_power = 1.0f;

//�R���X�g���N�^
NormalGhost::NormalGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Ghost,Skeleton_Ghost,Animation_Ghost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	player_{ nullptr }{
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
		hp_ = 1.0f;
		
}

//�X�V
void NormalGhost::update(float delta_time) {
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
	mesh_.change_motion(motion_);
	//���b�V�����X�V
	mesh_.update(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());

	//�d���y���̂��߃v���C���[�̍��W����-40���ꂽ�玀�S
	if (transform_.position().x <= player_->transform().position().x - 20.0f) {
		die();
	}
}

//�`��
void NormalGhost::draw() const {
	if (is_inside()) {
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.8f);
		//���b�V���̕`��
		mesh_.draw();
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
		//�Փ˔���̃f�o�b�O�\��
#ifdef _DEBUG
		collider().draw();
#endif
		if (state_ == State::Died) {
			world_->particle_manager()->death_smoke(transform_.position());
		}
	}
}

//�Փ˃��A�N�V����
void NormalGhost::react(Actor& other) {
	//�_���[�W���܂��͎��S���͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return;

	if (other.tag() == "PlayerTag") {
		DamageProp::do_attack(other, *this, atk_power);
	}
	
}

bool NormalGhost::on_hit(const Actor& other, float atk_power) {
	//�_���[�W���܂��͎��S���͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return false;

	if (other.tag() == "PlayerTag" || other.tag() == "PlayerAttack") {
		hp_-= atk_power;
		change_state(State::Damage, MotionDamage, false);
		return true;
	}
	return false;
}

//��Ԃ̍X�V
void NormalGhost::update_state(float delta_time) {
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
void NormalGhost::change_state(State state, GSuint motion, bool loop) {
	//���[�V�����ԍ��̍X�V
	motion_ = motion;
	//���[�V�����̃��[�v�w��
	motion_loop_ = loop;
	//��Ԃ̕ύX
	state_ = state;
	//��ԃ^�C�}�[�̏�����
	state_timer_ = 0.0f;
}

//�A�C�h����
void NormalGhost::idle(float delta_time) {
	if (is_move()) {
		change_state(State::Move, MotionRun);
		return;
	}
	change_state(State::Idle, MotionIdle);
}

//�ړ���
void NormalGhost::move(float delta_time) {
	if (transform_.position().x <= -10.0f) {
		change_state(State::Died, MotionDie, false);
		return;
	}
	GSvector3 velocity{-1.0f,0.0f,0.0f};
	velocity.z = 0.0f;
	velocity_ = velocity;
	transform_.translate(velocity_ * delta_time * Speed, GStransform::Space::World);
}

//�_���[�W��
void NormalGhost::damage(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Died,MotionDie,false);
	}
}

//���S��
void NormalGhost::died(float delta_time) {
	//���[�V�����I����Ɏ��S
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//�ړ�����
bool NormalGhost::is_move() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//��ʓ��ɂ�����ړ�����
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//�J�����̑O�x�N�g��
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle <= 45.0f);
}

//�J�����̓����ɂ��邩�H
bool NormalGhost::is_inside() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//��ʓ��ɂ�����ړ�����
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//�J�����̑O�x�N�g��
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle <= 45.0f);
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

//�t�B�[���h�Ƃ̏Փ˔���
void NormalGhost::collide_field() {
	//�ǂƂ̏Փ˔���
	BoundingSphere sphere{ collider().radius,transform().position() };
	GSvector3 center;
	if (world_->field()->collide(sphere, &center)) {
		center.z = 0.0f;
		transform_.position(center);
		change_state(State::Died, MotionDie,false);
	}
	Line line{ collider().center,GSvector3{collider().center.x - EnemyRadius,collider().center.y,0.0f} };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		velocity_.y *= 0.1f;
	}
}

//�A�N�^�[�Ƃ̏Փˏ���
void NormalGhost::collide_actor(Actor& other) {
	//z���W���������W�����߂�
	GSvector3 position = transform_.position();
	position.z = 0.0f;
	GSvector3 target = other.transform().position();
	target.z = 0.0f;
	//����Ƃ̋���
	float distance = GSvector3::distance(position,target);
	//�Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius + other.collider().radius;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	v.z = 0.0f;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}