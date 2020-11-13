#include "SurogSakones.h"
#include"IWorld.h"
#include"Field.h"
#include"Player.h"
#include"Assets.h"
#include"PsycokinesisBullet.h"

enum {
	MotionIdol1 = 0,
	MotionIdol2 = 1,
	MotionMove = 2,
	MotionScytheAttack = 3,
	MotionAttack2 = 5,
	MotionAttack3 = 6,
	MotionDamage1 = 7,
	MotionDamage2 = 8,
	MotionDying = 9
};

const float MoveSpeed{ 0.35f };
const float ScytheRange{ 0.5f };

SurogSakones::SurogSakones(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_SurogSakones, Skeleton_SurogSakones, Animation_SurogSakones, MotionIdol1 }
{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "SurogSakones";
	transform_.position(position);
	collider_ = BoundingSphere{ 5,GSvector3::up() * 5.0f };
	state_ = State::Idol;
	hp_ = 100.0f;
	transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-90.0f,0.0f }));

	mesh_.transform(transform_.localToWorldMatrix());

	move_pos_.push_back(transform().position());
	move_pos_.push_back(transform().position() - GSvector3{ 50.0f,0.0f,0.0f });

	destination_ = move_pos_[0];
	//gsRandamize();
	float a[10], b[10];
	for (int i = 0; i < 10; ++i) {
		a[i] = gsRand(10, 100);
		b[i] = gsRand(10, 100);
	}
}
void SurogSakones::update(float delta_time) {
	if (gsGetKeyTrigger(GKEY_1)) {
		change_state(State::Idol, MotionIdol1);
	}
	if (gsGetKeyTrigger(GKEY_2)) {
		change_state(State::Idol, MotionIdol2);
	}
	if (gsGetKeyTrigger(GKEY_3)) {
		change_state(State::Move, MotionMove);
		if (destination_ == move_pos_[0])destination_ = move_pos_[1];
		else destination_ = move_pos_[0];
	}
	if (gsGetKeyTrigger(GKEY_4)) {
		change_state(State::ScytheAttack, MotionScytheAttack);
	}
	if (gsGetKeyTrigger(GKEY_5)) {
		change_state(State::PsycoAttack_1, MotionAttack2);
	}
	if (gsGetKeyTrigger(GKEY_6)) {
		change_state(State::PsycoAttack_2, MotionAttack3);
	}
	if (gsGetKeyTrigger(GKEY_7)) {
		change_state(State::Stun, MotionDamage1);
	}
	if (gsGetKeyTrigger(GKEY_8)) {
		change_state(State::Stun, MotionDamage2);
	}
	if (gsGetKeyTrigger(GKEY_9)) {
		change_state(State::Dying, MotionDying);
	}
	if (gsGetKeyTrigger(GKEY_SPACE)) {
		if (state_ != State::Stun)Damage();
		if (hp_ <= 0)change_state(State::Dying, MotionDying);
	}
	update_state(delta_time);
	mesh_.change_motion(motion_);
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());

	if (gsGetKeyState(GKEY_RCONTROL)) {
		destination_ = move_pos_[0];
	}
	if (gsGetKeyState(GKEY_LCONTROL)) {
		destination_ = move_pos_[1];
	}
}
void SurogSakones::late_update(float delta_time) {
	prev_flip_ = flip_;
}
void SurogSakones::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		Damage();
	}
}
void SurogSakones::update_state(float delta_time) {
	//状態によって切り替え
	switch (state_) {
	case State::Appear:appear(delta_time); break;
	case State::Idol:idol(delta_time); break;
	case State::Move:move(delta_time); break;
	case State::Turn:turn(delta_time); break;
	case State::ScytheAttack:scythe_attack(delta_time); break;
	case State::PsycoAttack_1:psyco1_attack(delta_time); break;
	case State::PsycoAttack_2:psyco2_attack(delta_time); break;
	case State::Stun:stun(delta_time); break;
	case State::Dying:dying(delta_time); break;
	}
	state_timer_ += delta_time;
}

void SurogSakones::appear(float delta_time) {

}
void SurogSakones::idol(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) {
		change_state(State::Idol, MotionIdol1);
		return;
	}

	if (is_scythe_attack(player)) {
		change_state(State::ScytheAttack, MotionScytheAttack);
		return;
	}

	if (is_psyco1_attack(player)) {
		change_state(State::PsycoAttack_1, MotionAttack2);
		return;
	}
	if (is_psyco2_attack(player)) {
		change_state(State::PsycoAttack_2, MotionAttack3);
		return;
	}

	change_state(State::Idol, MotionIdol1);
}
void SurogSakones::scythe_attack(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		idol(delta_time);
	}
}
void SurogSakones::psyco1_attack(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		idol(delta_time);
	}
}
void SurogSakones::psyco2_attack(float delta_time) {	
	if (state_timer_ >= mesh_.motion_end_time()) {
		idol(delta_time);
		pshychokinesis(transform_.position());
	}
}
void SurogSakones::stun(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(prev_state_, prev_motion_);
	}
}
void SurogSakones::dying(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		die();
	}
}
void SurogSakones::turn(float delta_time) {
	if (!flip_)to_rotate_ = GSvector3{ 0.0f, 90.0f ,0.0f };
	else to_rotate_ = { 0.0f, -90.0f ,0.0f };
	if (GSquaternion::angle(transform_.rotation(), GSquaternion::euler(to_rotate_)) >= 5.0f) {
		transform_.rotate(GSvector3{ 0.0f,1.0f,0.0f }, 5.0f / delta_time, GStransform::Space::World);
	}
	else {
		if (state_timer_ >= mesh_.motion_end_time()) {
			transform_.rotation(GSquaternion::euler(to_rotate_));
			flip_ = !flip_;
			idol(delta_time);
		}
	}
}
void SurogSakones::move(float delta_time) {
	if (GSvector3::distance(transform_.position(), destination_) <= 0.5f) {
		transform_.position(destination_);
		change_state(State::Turn, MotionScytheAttack);
	}
	else if (transform_.position() < destination_) {
		velocity_ = GSvector3{ 1.0f,0.0f,0.0f }*MoveSpeed;
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	}
	else if (transform_.position() > destination_) {
		velocity_ = GSvector3{ -1.0f,0.0f,0.0f }*MoveSpeed;
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	}
}
void SurogSakones::change_state(State state, GSuint motion) {
	prev_motion_ = motion_;
	motion_ = motion;
	prev_state_ = state_;
	state_ = state;
	state_timer_ = 0.0f;
}
void SurogSakones::draw()const {
	mesh_.draw();
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
	gsTextPos(0.0f, 40.0f);
	gsDrawText("SurogSakones:スケール(%f,%f,%f)", transform_.lossyScale().x, transform_.lossyScale().y, transform_.lossyScale().z);
	gsTextPos(0.0f, 60.0f);
	gsDrawText("SurogSakones:モーション番号(%d)", motion_);
	gsTextPos(0.0f, 80.0f);
	gsDrawText("状態：(%d)", state_);
}

/**
* PurPose 攻撃を食らう処理
* 改良予定
*/
void SurogSakones::Damage() {
	hp_ -= 10;
	change_state(State::Stun, MotionDamage1);
}

void SurogSakones::pshychokinesis(const GSvector3& position) {
	Actor* player = world_->find_actor("Player");
	//プレイヤーが憑依中かしらべ、憑依中じゃなかったらreturnする処理を後々追加
	//プレイヤーがヌルでないか
	if (player != nullptr) {
		//プレイヤーのベクトルを求める
		GSvector3 to_player = (player->transform().position() - transform().position()).normalized();
		//球を出す処理
		world_->add_actor(new PsycokinesisBullet(world_, position,GSvector3::right()+GSvector3::up()*75.0f));
		world_->add_actor(new PsycokinesisBullet(world_, position,GSvector3::up()*75.0f));
		world_->add_actor(new PsycokinesisBullet(world_, position,GSvector3::up()*75.0f));
	}
}

void SurogSakones::move_attack(float delta_time) {

}

void SurogSakones::turn(float delta_time, float slow_value, bool flip) {
	if (flip)to_rotate_ = GSvector3{ 0.0f, 90.0f ,0.0f };
	else to_rotate_ = { 0.0f, -90.0f ,0.0f };
	motion_ = MotionScytheAttack;
	if (GSquaternion::angle(transform_.rotation(), GSquaternion::euler(to_rotate_)) >= 5.0f) {
		transform_.rotate(GSvector3{ 0.0f,1.0f,0.0f }, 5.0f / delta_time, GStransform::Space::World);
	}
	else {
		transform_.rotation(GSquaternion::euler(to_rotate_));
		state_ = State::Idol;
	}
}
float SurogSakones::target_distance(const Actor* other) {
	return (other->transform().position() - transform_.position()).magnitude();
}
float SurogSakones::target_signed_angle(const Actor* other) {
	GSvector3 to_player = other->transform().position() - transform_.position();
	GSvector3 forward = transform_.forward();

	to_player.z = 0.0f;
	forward.z = 0.0f;

	return GSvector3::signed_angle(to_player, forward);
}
float SurogSakones::target_angle(const Actor* other) {
	return std::abs(target_signed_angle(other));
}

bool SurogSakones::is_scythe_attack(const Actor* other) {
	return (target_distance(other) <= ScytheRange && target_signed_angle(other) <= 60.0f);
}
bool SurogSakones::is_psyco1_attack(const Actor* other) {
	return false;
}
bool SurogSakones::is_psyco2_attack(const Actor* other) {
	return false;
	//プレイヤーの状態が憑依しているなら
	//Player* player = (Player*)other;
}