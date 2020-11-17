#include"Poltergeist.h"
#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
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
const float EnemyHeight{ 0.75f };
//�G�l�~�[�̔��a
const float EnemyRadius{ 0.5f };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//�d��
const float Gravity{ -0.016f };

//�R���X�g���N�^
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_Poltergeist,Mesh_CarGhost,Mesh_CarGhost,MotionIdle},
	motion_{MotionIdle} ,
	motion_loop_{true},
	state_{ State::Idle },
	state_timer_{0.0f},
	player_{nullptr},
	hp_{1} {
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "Poltergeist";
	//�^�O���̐ݒ�
	tag_ = "EnemyTag";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	transform_.position(position);
	transform_.localScale(GSvector3{ 0.3f,0.3f,0.3f });
	mesh_.transform(transform_.localToWorldMatrix());
}

//�X�V
void Poltergeist::update(float delta_time) {
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
	//���b�V�����X�V
	mesh_.update(delta_time);
	//�s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
	//�ˌ��^�C�}�[�X�V
	shootiong_timer_ -= delta_time;

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
	if (other.tag() == "PlayerAttackTag") {
		hp_--;
		if (hp_ <= 0) {
			//�_���[�W��ԂɕύX
			change_state(State::Damage, MotionDamage,false);
		} else {
			//�U���̐i�s�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//�_���[�W��ԂɕύX
			change_state(State::Damage, MotionDamage,false);
		}
		return;
	}
	//�v���C���[�܂��̓G�l�~�[�ɏՓ˂���
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
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
	//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset,0.0f };
	GSvector3 intersect;//�n�ʂƂ̌�_
	if (world_->field()->collide(line, &intersect)) {
		//���������_����y���W�̂ݕ␳����
		position.y = intersect.y;
		transform_.position(position);
	}
}

//�A�N�^�[�Ƃ̏Փˏ���
void Poltergeist::collide_actor(Actor& other) {
	//y���W���������W�����߂�
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
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