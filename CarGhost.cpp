#include"CarGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"AttackCollider.h"
#include"ActorProp.h"
#include"Camera.h"

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
const float MoveDistance{ 5.0f };
//�U������p�x
const float TurnAngle{ 5.0f };
//�G�l�~�[�̍���
const float EnemyHeight{ 0.75f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 0.5f };
//�G�l�~�[�̉��̃I�t�Z�b�g
const float EnemySideOffset{ 0.6f };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//�X�s�[�h
const float Speed{ 0.05f };
//x���W�̎��S���W
const float LimitDistance_x{ 100.0f };
//y���W�̎��S���W
const float LimitDistance_y{ 100.0f };
//�d��
const float Gravity{ -0.0016f };


//�R���X�g���N�^
CarGhost::CarGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Poltergeist,Skeleton_CarGhost,Animation_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	player_{ nullptr },
	is_turn_{ false },
	is_hit_{ false } {
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "CarGhost";
	//�^�O���̐ݒ�
	tag_ = "EnemyTag";
	//ActorProp���p�����Ă��邩�H
	hit_ = true;
	//�̗͂̐ݒ�
	hp_ = 1.0f;
	//�U���͂̐ݒ�
	atk_power_ = 1.0f;
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
	//�J�����̊O���ɂ���Ɖ������Ȃ�
	if (is_inside()) {
		//�v���C���[������
		player_ = world_->find_actor("Player");
		//��Ԃ̍X�V
		update_state(delta_time);
		//�d�͂��X�V
		velocity_.y += Gravity * delta_time;
		//�d�͂�������
		transform_.translate(0.0f, velocity_.y, 0.0f);
		//�t�B�[���h�Ƃ̏Փ˔���
		collide_field();
		//���[�V�����ύX
		mesh_.change_motion(motion_);
		//���b�V���̍X�V
		mesh_.update(delta_time);
		//�s���ݒ�
		mesh_.transform(transform_.localToWorldMatrix());
	}
}

//�`��
void CarGhost::draw() const {
	mesh_.draw();

#ifdef _DEBUG
	collider().draw();
	gsFontParameter(GS_FONT_BOLD, 16, "�l�r �S�V�b�N");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("CarGhost:�x�N�g��(%f,%f,%f)", velocity_.x, velocity_.y, velocity_.z);
	gsTextPos(0.0f, 40.0f);
	gsDrawText("CarGhost:���W(%f,%f,%f)", transform_.position().x, transform_.position().y, transform_.position().z);
#endif

}

//�Փ˃��A�N�V����
void CarGhost::react(Actor& other) {
	//�_���[�W���܂��͎��S���͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died) return;

	else if (other.tag() == "PlayerTag") {//�v���C���[�ƏՓ˂����ꍇ
		is_hit_ = true;
		ActorProp::do_attack(other, *this, atk_power_);
	}
}

//�U�����󂯂�
void CarGhost::on_hit(const Actor& other, float atk_power) {
	//�_���[�W���܂��͎��S���͉������Ȃ�
	if (state_ == State::Damage || state_ == State::Died)return;

	if (other.tag() == "PlayerAttack") { //�v���C���[�̍U���ƏՓ˂����ꍇ
		hp_ -= atk_power;
		if (hp_ <= 0) {
			//���S��ԂɕύX
			change_state(State::Died, MotionDie, false);
		}
		else {
			//�U���̐i�s�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//�_���[�W��ԂɕύX
			change_state(State::Damage, MotionDamage, false);
		}
		return;
	}
}

//��Ԃ̍X�V
void CarGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//��ԃ^�C�}�[�X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void CarGhost::change_state(State state, GSuint motion, bool loop) {
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
	//�v���C���[�����������H
	if (is_move()) {
		change_state(State::Move, MotionRun);
		return;
	}
	//�����Ȃ���΃A�C�h��
	change_state(State::Idle, MotionIdle);
}

//�ړ�
void CarGhost::move(float delta_time) {
	GSvector3 velocity = GSvector3::zero();
	if (!is_hit_) {
		if (is_turn()) {
			change_state(State::Turn, MotionIdle);
			return;
		}
		velocity = GSvector3{ to_target().x,0.0f,0.0f };
		velocity_ = velocity;
		//�^�[�Q�b�g�����̊p�x�����߂�
		float angle = CLAMP(target_signed_angle(), -TurnAngle / 3, TurnAngle / 3);
		//�^�[�Q�b�g����������
		transform_.rotate(0.0f, angle, 0.0f);
	}
	else {
		velocity = GSvector3{ transform_.forward().x,0.0f,0.0f };
		velocity_ = velocity;
		/*GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(velocity_), 12.0f * delta_time);
		transform_.rotation(rotation);*/
	}
	//�ړ�
	transform_.translate(velocity_ * Speed, GStransform::Space::World);

}

//�^�[��
void CarGhost::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//�U��������[�V�������I��������ړ����ɑJ��
		change_state(State::Move, MotionRun);
	}
	else {
		//�U��������[�V���������Ȃ���^�[�Q�b�g����������
		float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.0f, angle, 0.0f);
		/*GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(-velocity_), 5.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);*/
	}
}

//�_���[�W
void CarGhost::damage(float delta_time) {
	if (state_timer_ < mesh_.motion_end_time()) {
		//�m�b�N�o�b�N����
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
		velocity_ -= GSvector3{ velocity_.x,velocity_.y,0.0f } *0.5f * delta_time;
	}
	//���[�V�������I��������A�C�h����
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Move, MotionRun);
	}
}

//���S
void CarGhost::died(float delta_time) {
	//���[�V�������I�������玀�S
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//�ړ�����
bool CarGhost::is_move()const {
	//�ړ��������O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance() <= MoveDistance) && (target_angle() <= 20.0f);
}

//�U���������
bool CarGhost::is_turn() const {
	//�U��������������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20�x�ȉ����H
	return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

//�J�����̓����ɂ��邩
bool CarGhost::is_inside() const {
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
	GSvector3 player = player_->transform().position();
	return (player - transform_.position()).normalized();
}

//�t�B�[���h�Ƃ̏Փˏ���
void CarGhost::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	GSvector3 center;//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		center.y = transform_.position().y;
		center.z = 0.0f;
		//�␳��̍��W�ɕύX����
		transform_.position(center);
	}
	//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
	GSvector3 position2 = transform_.position();
	Line line2;
	line2.start = position2 + collider_.center;
	line2.end = position2 + GSvector3{ 0.0f,-FootOffset,0.0f };
	GSvector3 intersect2;//�n�ʂƂ̌�_
	if (world_->field()->collide(line2, &intersect2)) {
		//��_�����_����y���W�̂ݕ␳����
		position2.y = intersect2.y;
		position2.z = 0.0f;
		//���W��ύX����
		transform_.position(position2);
		//�d�͂�������
		velocity_.y = 0.0f;
	}
	if (is_hit_) {
		Line line;
		line.start = collider().center;
		line.end = collider().center + GSvector3{ -EnemySideOffset,0.0f,0.0f };
		GSvector3 intersect;
		GSvector3 position = transform_.position();
		//�����𒲂ׂ�
		if (world_->field()->collide(line, &intersect)) {
			change_state(State::Turn, MotionIdle);
			position.x += 0.1f;
			position.y = transform_.position().y;
			position.z = 0.0f;
			transform_.position(position);
		}
		//�E���𒲂ׂ�
		line.start = collider().center;
		line.end = collider().center + GSvector3{ EnemySideOffset,0.0f,0.0f };
		if (world_->field()->collide(line, &intersect)) {
			change_state(State::Turn, MotionIdle);
			position.x -= 0.1f;
			position.y = transform_.position().y;
			position.z = 0.0f;
			transform_.position(position);
		}
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
	v.z = 0.0f;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}
