#include"CarGhost.h"
#include"IWorld.h"
#include"Assets.h"


//コンストラクタ
CarGhost::CarGhost(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "CarGhost";
	tag_ = "EnemyTag";
	transform_.position(position);
	velocity_ = { 0.0f,0.0f,0.0f };
}

//更新
void CarGhost::update(float delta_time) {
	update_state(delta_time);
}

//描画
void CarGhost::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glScaled(50.0f, 50.0f, 50.0f);
	gsDrawMesh(Mesh_CarGhost);
	glPopMatrix();
}

//衝突リアクション
void CarGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
	}
}

//状態の更新
void CarGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Move: move(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//状態タイマー更新
	state_timer_ += delta_time;
}

//状態の変更
void CarGhost::change_state(State state, GSuint motion) {
	state_ = state;
	state_timer_ = 0.0f;
}

//移動
void CarGhost::move(float delta_time) {
	//プレイヤーを探す
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
		//スピードを上げる
		speed_ = 1.5f;
	}
	//移動
	//transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//攻撃
void CarGhost::attack(float delta_time) {

}

//ダメージ
void CarGhost::damage(float delta_time) {
	//プレイヤーのatkを引く
	hp_ -= 1.0f;
	if (hp_ <= 0) {
		//Dieに状態変更
	}
}

//死亡
void CarGhost::died(float delta_time) {
	die();
}