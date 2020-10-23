#include"CarGhost.h"
#include"IWorld.h"
#include"Assets.h"


//�R���X�g���N�^
CarGhost::CarGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "CarGhost";
	tag_ = "EnemyTag";
	transform_.position(position);
	velocity_ = { 0.0f,0.0f,0.0f };
}

//�X�V
void CarGhost::update(float delta_time) {
	update_state(delta_time);
}

//�`��
void CarGhost::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glScaled(50.0f, 50.0f, 50.0f);
	gsDrawMesh(Mesh_CarGhost);
	glPopMatrix();
}

//�Փ˃��A�N�V����
void CarGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
	}
}

//��Ԃ̍X�V
void CarGhost::update_state(float delta_time) {
	switch (state_) {
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
	state_ = state;
	state_timer_ = 0.0f;
}

//�ړ�
void CarGhost::move(float delta_time) {
	//�v���C���[��T��
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		//�X�s�[�h���グ��
		speed_ = 1.5f;
	}
	//�ړ�
	//transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//�U��
void CarGhost::attack(float delta_time) {

}

//�_���[�W
void CarGhost::damage(float delta_time) {
	//�v���C���[��atk������
	hp_ -= 1.0f;
	if (hp_ <= 0) {
		//Die�ɏ�ԕύX
	}
}

//���S
void CarGhost::died(float delta_time) {
	die();
}