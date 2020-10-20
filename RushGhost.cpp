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
	//�v���C���[���
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {//�v���C���[�����[���h���ɂ�����

		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		if (to_player.y < 2) {
			velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
			speed_ = 1.5f;
		}
	}
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
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
		die();
	}
}