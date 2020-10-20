#include"RushGhost.h"
#include"IWorld.h"



//コンストラクタ
RushGhost::RushGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushEnemyF";
	tag_ = "EnemyTag";
	transform_.position(position);
	gsLoadMesh(10, "Ghost.msh");
}

//更新
void RushGhost::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		if (to_player.y < 2) {
			velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
			speed_ = 1.5f;
		}
	}
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//描画
void RushGhost::draw() const {
	glPushMatrix();
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	gsDrawMesh(10);
	glPopMatrix();
}

//衝突リアクション
void RushGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}