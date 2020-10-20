#include "SurogSakones.h"
#include"IWorld.h";
#include"Field.h";
#include"Player.h"

SurogSakones::SurogSakones(IWorld* world, const GSvector3& position) {
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "SurogSakones";
	transform_.position(position);
	collider_ = BoundingSphere(5);
	state_ = State::Normal;
	hp_ = 100.0f;

	move_pos_.push_back(transform().position());
	move_pos_.push_back(transform().position() - GSvector3{ 50.0f,0.0f,0.0f });
}
void SurogSakones::update(float delta_time) {
	if (hp_ <= 0)die();
	move(delta_time);	
	if (gsGetKeyState(GKEY_UP) == GS_TRUE) {
		destination_ = move_pos_[0];
	}
	if (gsGetKeyState(GKEY_DOWN) == GS_TRUE) {
		destination_ = move_pos_[1];
	}
}
void SurogSakones::draw()const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glPopMatrix();
	collider().draw();
}

/**
* PurPose 攻撃を食らう処理
* 改良予定
*/
void SurogSakones::Damage() {
	hp_ -= 10;
}

void SurogSakones::pshychokinesis(const GSvector3& position) {
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {
		//プレイヤーのベクトルを求める
		GSvector3 to_player = (player->transform().position() - transform().position()).normalized();
		//球を出す処理
	}
}
void SurogSakones::move(float delta_time) {
	if (GSvector3::distance(transform_.position(), destination_) <= 0.5f) {
		transform_.position(destination_);
		enable_collider_ = true;
	}
	else if (transform_.position() < destination_) {
		velocity_ = GSvector3{ 1.0f,0.0f,0.0f };
		transform_.translate(velocity_ * delta_time);
		enable_collider_ = false;
	}
	else if (transform_.position() > destination_) {
		velocity_ = GSvector3{ -1.0f,0.0f,0.0f };
		transform_.translate(velocity_ * delta_time);
		enable_collider_ = false;
	}
}

void SurogSakones::move_attack(float delta_time) {

}