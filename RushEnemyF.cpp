#include"RushEnemyF.h"
#include"IWorld.h"



//コンストラクタ
RushEnemyF::RushEnemyF(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushEnemyF";
	tag_ = "EnemyTag";
	transform_.position(position);
}

//更新
void RushEnemyF::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		speed_ = 2.0f;
	}
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//描画
void RushEnemyF::draw() const {

}

//衝突リアクション
void RushEnemyF::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}