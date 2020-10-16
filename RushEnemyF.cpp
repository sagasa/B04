#include"RushEnemyF.h"
#include"IWorld.h"



//�R���X�g���N�^
RushEnemyF::RushEnemyF(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushEnemyF";
	tag_ = "EnemyTag";
	transform_.position(position);
}

//�X�V
void RushEnemyF::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		speed_ = 2.0f;
	}
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//�`��
void RushEnemyF::draw() const {

}

//�Փ˃��A�N�V����
void RushEnemyF::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}