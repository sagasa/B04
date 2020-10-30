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
const float AttackDistance{ 1.2f };
//�ړ�����̋���
const float WalkDistance{ 10.0f };
//�U������p�x
const float TurnAngle{ 2.5f };

//�R���X�g���N�^
CarGhost::CarGhost(IWorld* world, const GSvector3& position):
	mesh_{Mesh_CarGhost,Skeleton_CarGhost,Animation_CarGhost,MotionIdle},
	motion_{ MotionIdle },
	state_{ State::Idle } {
	world_ = world;
	name_ = "CarGhost";
	tag_ = "EnemyTag";
	transform_.position(position);
	player_ = world_->find_actor("Player");
}

//�X�V
void CarGhost::update(float delta_time) {
	//��Ԃ̍X�V
	update_state(delta_time);
	//���[�V�����ύX
	mesh_.change_motion(motion_);
	//���b�V���̍X�V1
	mesh_.update(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
}

//�`��
void CarGhost::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glScaled(0.2f, 0.2f, 0.2f);
	transform_.position();
	mesh_.draw();
	glPopMatrix();
}

//�Փ˃��A�N�V����
void CarGhost::react(Actor& other) {
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
	
	if (player_ != nullptr) {
		GSvector3 to_player = (player_->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		//�X�s�[�h���グ��
		//speed_ = 0.25f;
	}
	//�ړ�
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//�U��
void CarGhost::attack(float delta_time) {
	//���[�V�������I��������Move��ԂɑJ��
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
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
	if (state_timer_ >= mesh_.motion_end_time()-30.0f) {
		die();
	}
}

//�U���������
bool CarGhost::is_turn()const {
	return(target_distance() <= TurnDistance)&& (target_angle() <= 20.0f);
}

//�U������
bool CarGhost::is_attack()const {
	//�U�����������O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����20.0�x�ȉ����H
	return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

//�ړ�����
bool CarGhost::is_move()const {
	//�ړ��������O�����̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����100.0�x�ȉ����H
	return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

//�O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�(�����t��)
float CarGhost::target_signed_angle()const {
	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target  = player_->transform().position() - transform_.position();
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