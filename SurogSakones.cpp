#include "SurogSakones.h"

#include "IWorld.h"
#include "Field.h"
#include "Player.h"
#include "Assets.h"
#include "PsycokinesisBullet.h"
#include "Line.h"

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

const float MoveSpeed{ 0.035f };
const float SlowMoveSpeed{ 0.015f };
const float FootOffset{ 0.1f };
const float Gravity{ 0.2f };
const float TurnDistance{ 2.0f };
const float MinMoveDistance{ 3.5f };
const float SarchMoveDistance{ 5.5f };
const float MaxMoveDistance{ 10.0f };

const float ScytheRange{ 1.25f };
const float Psyco1Range{ 5.5f };

//アタック時のフレーム数
const float ScytheAttackFlame{ 20.0f };
const float Psyco1AttackFlame{ 20.0f };
const float Psyco2AttackFlame{ 20.0f };

//アタック時のクールタイム
const float ScytheAttackCoolTime{ 30.0f };
const float Psyco1AttackCoolTime{ 60.0f };
const float Psyco2AttackCoolTime{ 30.0f };

SurogSakones::SurogSakones(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_SurogSakones, Skeleton_SurogSakones, Animation_SurogSakones, MotionIdol1 },
	player_{ nullptr },
	move_way_{ Move::Normal },
	loop_{ false },
	player_cross_{ false }
{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "SurogSakones";
	transform_.position(position);
	collider_ = BoundingSphere{ 0.75,GSvector3::up() * 1.0f };
	state_ = State::Idol;
	hp_ = 100.0f;
	transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-90.0f,0.0f }));

	mesh_.transform(transform_.localToWorldMatrix());

	move_pos_.push_back(transform().position());
	move_pos_.push_back(transform().position() - GSvector3{ 5.0f,0.0f,0.0f });
}
void SurogSakones::update(float delta_time) {
	if (player_ == nullptr)
	{
		player_ = world_->find_actor("Player");
	}
	if (gsGetKeyTrigger(GKEY_1)) {
		change_state(State::Idol, MotionIdol1);
	}
	if (gsGetKeyTrigger(GKEY_2)) {
		change_state(State::Idol, MotionIdol2);
	}
	if (gsGetKeyTrigger(GKEY_4)) {
		change_state(State::Attack, MotionScytheAttack);
	}
	if (gsGetKeyTrigger(GKEY_5)) {
		change_state(State::Attack, MotionAttack2);
	}
	if (gsGetKeyTrigger(GKEY_6)) {
		change_state(State::Attack, MotionAttack3);
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
		if (hp_ <= 0)change_state(State::Dying, MotionDying, false);
	}
	update_state(delta_time);
	transform_.translate(GSvector3{ 0.0f,-Gravity,0.0f }*delta_time);
	collide_field();
	mesh_.change_motion(motion_, loop_);
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());
}
void SurogSakones::late_update(float delta_time) {
	prev_flip_ = flip_;
}
void SurogSakones::react(Actor& other) {
}
void SurogSakones::update_state(float delta_time) {
	//状態によって切り替え
	switch (state_) {
	case State::Appear:appear(delta_time); break;
	case State::Idol:idol(delta_time); break;
	case State::Move:move(delta_time); break;
	case State::Turn:turn(delta_time); break;
	case State::Attack:attack(delta_time); break;
	case State::Stun:stun(delta_time); break;
	case State::Dying:dying(delta_time); break;
	}
	state_timer_ += delta_time;
	attack_timer_ += delta_time;

	//クールタイムの計算(攻撃時に値を入れられる、0未満になるまで攻撃できない)
	cool_timer_ -= delta_time;
}

void SurogSakones::appear(float delta_time) {

}
void SurogSakones::idol(float delta_time) {
	if (player_ == nullptr)
	{
		change_state(State::Idol, MotionIdol1);
	}
	if (is_turn(player_))
	{
		change_state(State::Turn, MotionScytheAttack);
		return;
	}
	if (is_move(player_))
	{
		change_state(State::Move, MotionMove);
		return;
	}

	change_state(State::Idol, MotionIdol2);
}
void SurogSakones::attack(float delta_time)
{
	if (state_timer_ >= mesh_.motion_end_time())
	{
		psyco1_attack_flag_ = false;
		psyco2_attack_flag_ = false;
		scythe_attack_flag_ = false;
		switch (motion_)
		{
		case MotionScytheAttack:cool_timer_ = ScytheAttackCoolTime; break;
		case MotionAttack2:cool_timer_ = Psyco1AttackCoolTime; break;
		case MotionAttack3:cool_timer_ = Psyco2AttackCoolTime; break;
		}
		idol(delta_time);
		return;
	}
	switch (motion_)
	{
	case MotionScytheAttack:scythe_attack(delta_time); break;
	case MotionAttack2:psyco1_attack(delta_time); break;
	case MotionAttack3:psyco2_attack(delta_time); break;
	}
}
void SurogSakones::scythe_attack(float delta_time) {
	if (!scythe_attack_flag_ && attack_timer_ >= ScytheAttackFlame)
	{
		scythe_attack_flag_ = true;
		pshychokinesis(transform_.position() + GSvector3::up() * 2.0f, GSvector3::up() * 3.0f);
	}
}
void SurogSakones::psyco1_attack(float delta_time) {
	if (!psyco1_attack_flag_ && attack_timer_ >= ScytheAttackFlame)
	{
		psyco1_attack_flag_ = true;
		pshychokinesis(transform_.position() + GSvector3::up() * 2.0f, GSvector3::up() * 4.0f);
	}
}
void SurogSakones::psyco2_attack(float delta_time) {
	if (!psyco2_attack_flag_ && attack_timer_ >= ScytheAttackFlame)
	{
		psyco2_attack_flag_ = true;
		pshychokinesis(transform_.position() + GSvector3::up() * 2.0f, GSvector3::up() * 3.0f);
	}
}
void SurogSakones::stun(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		idol(delta_time);
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
			player_cross_ = false;
			idol(delta_time);
		}
	}
}
void SurogSakones::move(float delta_time) {
	if (is_scythe_attack(player_))
	{
		scythe_attack();
		return;
	}
	if (is_psyco1_attack(player_))
	{
		psyco1_attack();
		return;
	}
	if (is_psyco2_attack(player_))
	{
		psyco2_attack();
		return;
	}
	if(target_distance(player_)<=MinMoveDistance)
	{
		move_way_ = Move::Normal;
	}
	else if(target_distance(player_) > MinMoveDistance&& target_distance(player_) <= SarchMoveDistance)
	{
		move_way_ = Move::Slowly;
	}
	else if(target_distance(player_)>SarchMoveDistance&&target_distance(player_)<=MaxMoveDistance)
	{
		move_way_ = Move::Normal;
	}
	if (cool_timer_ > 0.0f)move_way_ = Move::Slowly;
	if (is_turn(player_))
	{
		change_state(State::Turn, MotionScytheAttack);
		return;
	}
	switch (move_way_)
	{
	case Move::Normal:move_normal(delta_time); break;
	case Move::Slowly:move_slowly(delta_time); break;
	case Move::Fast:move_fast(delta_time); break;
	}	
}

void SurogSakones::move_normal(float delta_time)
{
	destination_ = player_->transform().position();

	GSvector3 vel = GSvector3::zero();
	if (transform_.position() < destination_) {
		vel = GSvector3{ 1.0f,0.0f,0.0f }*MoveSpeed;
		transform_.translate(vel * delta_time, GStransform::Space::World);
	}
	else if (transform_.position() > destination_) {
		vel = GSvector3{ -1.0f,0.0f,0.0f }*MoveSpeed;
		transform_.translate(vel * delta_time, GStransform::Space::World);
	}	
}
void SurogSakones::move_slowly(float delta_time)
{
	float distance = target_distance(player_);
	if(is_psyco1_attack(player_))
	{
		psyco1_attack();
		return;
	}
	if(distance<=MinMoveDistance&&distance>=ScytheRange)
	{
		if (flip_)
		{
			transform_.translate(GSvector3{ SlowMoveSpeed*1.2f,0.0f,0.0f }*delta_time, GStransform::Space::World);			
			return;
		}
		else
		{
			transform_.translate(GSvector3{ -SlowMoveSpeed*1.2f,0.0f,0.0f }*delta_time, GStransform::Space::World);
			return;
		}
	}
	if (distance<Psyco1Range && distance>MinMoveDistance)
	{
		if (flip_)
		{
			transform_.translate(GSvector3{ -SlowMoveSpeed,0.0f,0.0f }*delta_time, GStransform::Space::World);
			return;
		}
		else
		{
			transform_.translate(GSvector3{ SlowMoveSpeed,0.0f,0.0f }*delta_time, GStransform::Space::World);
			return;
		}
	}
}
void SurogSakones::move_fast(float delta_time)
{
	return;
}
void SurogSakones::change_state(State state, GSuint motion, bool loop) {
	prev_motion_ = motion_;
	motion_ = motion;
	prev_state_ = state_;
	state_ = state;
	state_timer_ = 0.0f;
	loop_ = loop;
	attack_timer_ = 0.0f;
}
void SurogSakones::draw()const {
	mesh_.draw();
	collider().draw();
	debug_draw();
}

void SurogSakones::debug_draw()const {
	// デバッグ表示
	gsFontParameter(GS_FONT_BOLD, 16, "ＭＳ ゴシック");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("SurogSakones:座標(%f,%f,%f)", transform_.position().x, transform_.position().y, transform_.position().z);
	gsTextPos(0.0f, 40.0f);
	gsDrawText("SurogSakones:ローテーション(%f,%f,%f)", transform_.rotation().x, transform_.rotation().y, transform_.rotation().z);
	gsTextPos(0.0f, 60.0f);
	gsDrawText("SurogSakones:スケール(%f,%f,%f)", transform_.lossyScale().x, transform_.lossyScale().y, transform_.lossyScale().z);
	gsTextPos(0.0f, 80.0f);
	gsDrawText("SurogSakones:モーション番号(%d)", motion_);
	gsTextPos(0.0f, 100.0f);
	gsDrawText("状態：(%d)", state_);
	gsTextPos(0.0f, 120.0f);
	gsDrawText("プレイヤーとの角度(%d)", target_posrelation(player_));
	gsTextPos(0.0f, 140.0f);
	gsDrawText("移動状態(%d)", move_way_);
}

void SurogSakones::scythe_attack()
{
	change_state(State::Attack, MotionScytheAttack,false);
	cool_timer_ = ScytheAttackCoolTime;
}

void SurogSakones::psyco1_attack()
{
	change_state(State::Attack, MotionAttack2,false);
	cool_timer_ = Psyco1AttackCoolTime;
}

void SurogSakones::psyco2_attack()
{
	change_state(State::Attack, MotionAttack3,false);
	cool_timer_ = Psyco2AttackCoolTime;
}

/**
* PurPose 攻撃を食らう処理
* 改良予定
*/
void SurogSakones::Damage() {
	hp_ -= 10;
	change_state(State::Stun, MotionDamage1);
}

void SurogSakones::pshychokinesis(const GSvector3& position, GSvector3 velocity) {
	Actor* player = world_->find_actor("Player");
	//プレイヤーが憑依中かしらべ、憑依中じゃなかったらreturnする処理を後々追加
	//プレイヤーがヌルでないか
	if (player != nullptr) {
		//プレイヤーのベクトルを求める
		GSvector3 to_player = (player->transform().position() - transform().position()).normalized();
		//球を出す処理
		world_->add_actor(new PsycokinesisBullet(world_, position, velocity));
	}
}

void SurogSakones::move_attack(float delta_time) {

}

//ターゲットとの距離
float SurogSakones::target_distance(const Actor* other)const {
	if (other == nullptr)return 10000.0f;
	return (other->transform().position() - transform_.position()).magnitude();
}
//符号ありの角度
float SurogSakones::target_signed_angle(const Actor* other)const {
	if (other == nullptr)return 0.0f;
	GSvector3 to_player = other->transform().position() - transform_.position();
	GSvector3 forward = transform_.forward();

	to_player.z = 0.0f;
	forward.z = 0.0f;

	return GSvector3::signed_angle(to_player, forward);
}
//符号なしの角度
float SurogSakones::target_angle(const Actor* other)const {
	return std::abs(target_signed_angle(other));
}
//位置関係(左だったら負、右だったら正)
//nullptrだったら負
bool SurogSakones::target_posrelation(const Actor* other)const
{
	if (other == nullptr)return false;
	return (transform_.position().x - other->transform().position().x) <= 0;
}

bool SurogSakones::is_scythe_attack(const Actor* other) {
	if (other == nullptr)return false;
	return (target_distance(other) <= ScytheRange && target_signed_angle(other) <= 60.0f&&cool_timer_<=0.0f);
}
bool SurogSakones::is_psyco1_attack(const Actor* other) {
	if (other == nullptr)return false;
	return (target_distance(other) <= Psyco1Range &&target_distance(other)>=MinMoveDistance&& cool_timer_ <= 0.0f);
}
bool SurogSakones::is_psyco2_attack(const Actor* other) {
	return false;
	//プレイヤーの状態が憑依しているなら
	//Player* player = (Player*)other;
}
bool SurogSakones::is_turn(const Actor* other)
{
	return (target_angle(other) >= 90.0f);
}

bool SurogSakones::is_move(const Actor* other)
{
	return (target_distance(other) <= MaxMoveDistance);
}

void SurogSakones::collide_field()
{
	GSvector3 center;
	if (world_->field()->collide(collider(), &center))
	{
		center.y = transform_.position().y;
		center.z = 0.0f;
		transform_.position(center);
	}

	GSvector3 position = transform_.position();
	GSvector3 intersect;
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset,0.0f };
	if (world_->field()->collide(line, &intersect))
	{
		position.y = intersect.y;
		transform_.position(position);
		velocity_.y = 0;
	}
}
