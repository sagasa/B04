#include"RushGhost.h"
#include"IWorld.h"



//コンストラクタ
RushGhost::RushGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "RushGhost";
	tag_ = "EnemyTag";
	transform_.position(position);
	
}

//更新
void RushGhost::update(float delta_time) {
	update_state(delta_time);
}

//描画
void RushGhost::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	gsDrawMesh(10);
	glPopMatrix();
}

//衝突リアクション
void RushGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
	}
}

//状態の更新
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Move: move(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Die: died(delta_time); break;
	}
	//状態タイマーの更新
	state_timer_ += delta_time;
}

//状態の変更
void RushGhost::change_state(State state, GSuint motion) {
	state_ = state;
	state_timer_ = 0.0f;
}

//移動
void RushGhost::move(float delta_time) {
	//プレイヤー情報
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {//プレイヤーがワールド内にいたら

		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		if (to_player.y < 2) {
			velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
			//スピードを上げる
			speed_ = 1.5f;
		}
	}
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//攻撃
void RushGhost::attack(float delta_time) {

}

//ダメージ
void RushGhost::damage(float delta_time) {
	//プレイヤーのatkを引く
	hp_ -= 1.0f;
	if (hp_ <= 0) {
		//Dieに状態変更
	}
}

//死亡
void RushGhost::died(float delta_time) {
	die();
}