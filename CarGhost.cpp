#include"CarGhost.h"
#include"IWorld.h"


//�R���X�g���N�^
CarGhost::CarGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushEnemyG";
	tag_ = "EnemyTag";
	transform_.position(position);
	velocity_ = { 0.0f,0.0f,0.0f };
}

//�X�V
void CarGhost::update(float delta_time) {
	//�v���C���[��T��
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		speed_ = 1.5f;
	}
	//�ړ�
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//�`��
void CarGhost::draw() const {

}

//�Փ˃��A�N�V����
void CarGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}