#include"CarGhost.h"
#include"IWorld.h"


//コンストラクタ
CarGhost::CarGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushEnemyG";
	tag_ = "EnemyTag";
	transform_.position(position);
	velocity_ = { 0.0f,0.0f,0.0f };
}

//更新
void CarGhost::update(float delta_time) {
	//プレイヤーを探す
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		speed_ = 1.5f;
	}
	//移動
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//描画
void CarGhost::draw() const {

}

//衝突リアクション
void CarGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}