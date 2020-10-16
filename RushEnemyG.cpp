#include"RushEnemyG.h"
#include"IWorld.h"


//コンストラクタ
RushEnemyG::RushEnemyG(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushEnemyG";
	tag_ = "EnemyTag";
	transform_.position(position);
	velocity_ = { 0.0f,0.0f,0.0f };
}

//更新
void RushEnemyG::update(float delta_time) {
	//プレイヤーを探す
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		speed_ = 2.0f;
	}
	//移動
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//描画
void RushEnemyG::draw() const {

}

//衝突リアクション
void RushEnemyG::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}