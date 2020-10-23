#include"RushGhost.h"
#include"IWorld.h"



//�R���X�g���N�^
RushGhost::RushGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushGhost";
	tag_ = "EnemyTag";
	transform_.position(position);
	
}

//�X�V
void RushGhost::update(float delta_time) {
	update_state(delta_time);
}

//�`��
void RushGhost::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	gsDrawMesh(10);
	glPopMatrix();
}

//�Փ˃��A�N�V����
void RushGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
	}
}

//��Ԃ̍X�V
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Move: move(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Die: died(delta_time); break;
	}
	//��ԃ^�C�}�[�̍X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void RushGhost::change_state(State state, GSuint motion) {
	state_ = state;
	state_timer_ = 0.0f;
}

//�ړ�
void RushGhost::move(float delta_time) {
	//�v���C���[���
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {//�v���C���[�����[���h���ɂ�����

		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		if (to_player.y < 2) {
			velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
			//�X�s�[�h���グ��
			speed_ = 1.5f;
		}
	}
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//�U��
void RushGhost::attack(float delta_time) {

}

//�_���[�W
void RushGhost::damage(float delta_time) {
	//�v���C���[��atk������
	hp_ -= 1.0f;
	if (hp_ <= 0) {
		//Die�ɏ�ԕύX
	}
}

//���S
void RushGhost::died(float delta_time) {
	die();
}