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

	destination_ = move_pos_[0];
}
void SurogSakones::update(float delta_time) {
	if (hp_ <= 0)die();

	switch (state_) {
	case State::Apper:appear_update(delta_time); break;
	case State::Normal:normal_update(delta_time); break;
	}
	if (gsGetKeyState(GKEY_RCONTROL)) {
		destination_ = move_pos_[0];
	}
	if (gsGetKeyState(GKEY_LCONTROL)) {
		destination_ = move_pos_[1];
	}
	turn(delta_time, GSvector3{ 0.0f,180.0f,0.0f });
}

void SurogSakones::appear_update(float delta_time) {

}
void SurogSakones::normal_update(float delta_time) {
	move(delta_time);
}
void SurogSakones::angry_update(float delta_time) {

}
void SurogSakones::stun_update(float delta_time) {

}
void SurogSakones::die_update(float delta_time) {


}
void SurogSakones::draw()const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glutWireCube(5);
	glPopMatrix();
	//collider().draw();

	debug_draw();
}

void SurogSakones::debug_draw()const {
	// デバッグ表示
	gsFontParameter(GS_FONT_BOLD, 16, "ＭＳ ゴシック");
	gsTextPos(0.0f, 0.0f);
	gsDrawText("SurogSakones:座標(%f,%f,%f)", transform_.position().x, transform_.position().y, transform_.position().z);
	gsTextPos(0.0f, 20.0f);
	gsDrawText("SurogSakones:ローテーション(%f,%f,%f)", transform_.rotation().x, transform_.rotation().y, transform_.rotation().z);
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
	//プレイヤーが憑依中かしらべ、憑依中じゃなかったらreturnする処理を後々追加
	//プレイヤーがヌルでないか
	if (player != nullptr) {
		//プレイヤーのベクトルを求める
		GSvector3 to_player = (player->transform().position() - transform().position()).normalized();
		//球を出す処理
	}
}
void SurogSakones::move(float delta_time) {
	if (GSvector3::distance(transform_.position(), destination_) <= 0.5f) {
		transform_.position(destination_);
	}
	else if (transform_.position() < destination_) {
		velocity_ = GSvector3{ 1.0f,0.0f,0.0f };
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	}
	else if (transform_.position() > destination_) {
		velocity_ = GSvector3{ -1.0f,0.0f,0.0f };
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	}
}

void SurogSakones::move_attack(float delta_time) {

}

void SurogSakones::turn(float delta_time, const GSvector3& toRotate, float slow_value) {
	transform_.localRotation(GSquaternion::lerp(transform_.rotation(), GSquaternion::euler(toRotate), (delta_time / slow_value)));
}