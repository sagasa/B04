#include"Poltergeist.h"
#include"IWorld.h"
#include"Assets.h"



Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) {
	world_ = world;
	name_ = "Poltergeist";
	tag_ = "EnemyTag";
	transform_.position(position);
}

//更新
void Poltergeist::update(float delta_time) {
	update_state(delta_time);
}

//描画
void Poltergeist::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glScalef(30.0f, 30.0f, 30.0f);
	gsDrawMesh(Mesh_Poltergeist);
	glPopMatrix();
}

//衝突リアクション
void Poltergeist::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		//ダメージ状態に変更
	}
}

//状態の更新
void Poltergeist::update_state(float delta_time) {
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
void Poltergeist::change_state(State state, GSuint motion) {
	state_ = state;
	state_timer_ = 0.0f;
}

//移動
void Poltergeist::move(float delta_time) {

}

//攻撃
void Poltergeist::attack(float delta_time) {

}

//ダメージ
void Poltergeist::damage(float delta_time) {
	hp_ -= 1.0f;
	if (hp_ <= 0) {
		//死亡状態に変更
	}
}

//死亡
void Poltergeist::died(float delta_time) {
	die();
}