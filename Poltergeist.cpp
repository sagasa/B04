#include"Poltergeist.h"
#include"PoltergeistBullet.h"
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
//�ړ�����̋�����
const float AttackDistance_x{ 100.0f };
//�ړ�����̋���x
const float AttackDistance_y{ 100.0f };
//�U������p�x
const float TurnAngle{ 2.5f };
//�G�l�~�[�̍���
const float EnemyHeight{ 1.0f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 30.0f };


//�R���X�g���N�^
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_Poltergeist,Skeleton_CarGhost,Animation_CarGhost,MotionIdle},
	motion_{MotionIdle} ,
	state_{ State::Idle } {
	world_ = world;
	name_ = "Poltergeist";
	tag_ = "EnemyTag";
	player_ = world_->find_actor("Player");
	transform_.position(GSvector3::zero());
	collider_ = BoundingSphere{ EnemyRadius,mesh_.bone_matrices(0).position()};
	mesh_.transform(transform_.localToWorldMatrix());
	transform_.position(position);
	transform_.localScale(GSvector3{ 0.5f,0.5f,0.5f });
	transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-45.0f,0.0f }));
	
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
	//�ˌ��^�C�}�[�X�V
	shootiong_timer_ -= delta_time;

}

//�`��
void Poltergeist::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	mesh_.draw();
	glPopMatrix();
	collider().draw();
}

//�Փ˃��A�N�V����
void Poltergeist::react(Actor& other) {
	if (other.tag() == "PlayerTag" && hp_ <= 0) {
		//�_���[�W��ԂɕύX
		change_state(State::Damage, MotionDamage);
	}
}

//��Ԃ̍X�V
void Poltergeist::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Turn: turn(delta_time); break;
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

//�A�C�h��
void Poltergeist::idle(float delta_time) {
	//�U�����邩�H
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
}

//�^�[��
void Poltergeist::turn(float delta_time) {

}

//�U��
void Poltergeist::attack(float delta_time) {
	//���[�V�����I����Ɉړ����ɑJ��
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
	if (shootiong_timer_ <= 0.0f) {
		GSvector3 target = to_target();
		//world_->add_actor(new PoltergeistBullet{ world_,transform_.position(),target });
		shootiong_timer_ = gsRandf(20.0f, 60.0f);
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
	if (state_timer_ >= mesh_.motion_end_time()-30.0f) {
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
	return (target_distance_x() <= AttackDistance_x) && (target_distance_y() <= AttackDistance_y) && (target_angle() <= 180.0f);
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

//�^�[�Q�b�g�Ƃ�x�̋��������߂�
float Poltergeist::target_distance_x() const {
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�Ƃ�y�̋��������߂�
float Poltergeist::target_distance_y() const {
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//�^�[�Q�b�g�����̃x�N�g�������߂�
GSvector3 Poltergeist::to_target() const {
	return (player_->transform().position() - transform_.position()).normalized();
}