#include "SurogSakones.h"


#include <iostream>
#include <typeinfo>


#include "IWorld.h"
#include "Field.h"
#include "Player.h"
#include "Assets.h"
#include "PsycokinesisBullet.h"
#include "AttackCollider.h"
#include "Line.h"
#include "ParticleManager.h"

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
const float MinMoveDistance{ 4.0f };
const float SarchMoveDistance{ 6.0f };
const float MaxMoveDistance{ 12.0f };

const float ScytheRange{ 2.5f };
const float Psyco1Range{ 5.5f };

//アタック時のフレーム数
const float ScytheAttackFlame{ 20.0f };
const float Psyco1AttackFlame{ 20.0f };
const float Psyco2AttackFlame{ 20.0f };

//アタック時のクールタイム
const float ScytheAttackCoolTime{ 30.0f };
const float Psyco1AttackCoolTime{ 120.0f };
const float Psyco2AttackCoolTime{ 30.0f };
const float TurnAttackCoolTime{ 30.0f };

const GSvector3 SmokePosition{ 0.0f,0.5f,0.0f };

const GSvector2 hp_position{ 800.0f,30.0f };
const float HP_Length{ 300.0f };
const float HP_3DLength{ 10.0f };

SurogSakones::SurogSakones(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_SurogSakones, Skeleton_SurogSakones, Animation_SurogSakones, MotionIdol1 },
	player_{ nullptr },
	move_way_{ Move::Normal },
	loop_{ false }
{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "SurogSakones";
	transform_.position(position);
	collider_ = BoundingSphere{ 0.85f,GSvector3::up() * 1.85f };
	state_ = State::Idol;
	hp_ = 15.0f;
	max_hp_ = hp_;
	transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-90.0f,0.0f }));

	mesh_.transform(transform_.localToWorldMatrix());
}
void SurogSakones::update(float delta_time) {
#ifdef _DEBUG
	debug_input();
#endif
	player_ = world_->find_actor("Player");
	if (player_ == nullptr) {
		player_ = world_->find_actor("PlayerPaladin");
		if (player_ == nullptr) return;
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
	if (other.tag() == "PlayerTag")
	{
		if(is_collide_actor())
		{
			collide_actor(other);
		}
	}
}
bool SurogSakones::on_hit(const Actor& attacker, float atk_power)
{
	if (state_ == State::Stun || state_ == State::Dying)return false;
	if (attacker.tag() == "PlayerAttack")
	{		
		hp_ -= atk_power;
		++stun_counter_;
		play_se_damage();
		if(hp_ <= 0.0f)
		{
			play_se_damage(true);
			change_state(State::Stun, MotionDamage1, false);
			flip();
			return true;
		}
		if (stun_counter_ >= 3 && motion_ != MotionScytheAttack)
		{
			stun_counter_ = 0;
			change_state(State::Stun, MotionDamage1, false);
			flip();
		}
		
		psyco1_attack_flag_ = false;
		psyco2_attack_flag_ = false;
		scythe_attack_flag_ = false;

		return true;
	}
	return false;
}

void SurogSakones::flip()
{
	//攻撃したほうを向く
	if (target_posrelation(player_))
	{
		transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,90.0f,0.0f }));
		flip_ = true;
	}
	else
	{
		transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-90.0f,0.0f }));
		flip_ = false;
	}
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
		return;
	}
	if (is_turn(player_))
	{
		turn();
		return;
	}
	if (is_move(player_))
	{
		change_state(State::Move, MotionMove);
		return;
	}

	change_state(State::Idol, MotionIdol1);
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
	//不要
	/*switch (motion_)
	{
	case MotionScytheAttack:scythe_attack(delta_time); break;
	case MotionAttack2:psyco1_attack(delta_time); break;
	case MotionAttack3:psyco2_attack(delta_time); break;
	}*/
}
void SurogSakones::scythe_attack(float delta_time) {
	if (!scythe_attack_flag_ && attack_timer_ >= ScytheAttackFlame)
	{
		scythe_attack_flag_ = true;
	}
}
void SurogSakones::psyco1_attack(float delta_time) {
	if (!psyco1_attack_flag_)
	{
		world_->particle_manager()->death_smoke(
			transform_.position() + GSvector3::up() * 3.0f
		);
	}
	if (!psyco1_attack_flag_ && attack_timer_ >= ScytheAttackFlame)
	{
		psyco1_attack_flag_ = true;
		generate_pshychokinesis(
			transform_.position() + GSvector3::up() * 3.0f,
			GSvector3::up() * 4.0f
		);
	}
}
void SurogSakones::psyco2_attack(float delta_time) {
	if (!psyco2_attack_flag_ && attack_timer_ >= ScytheAttackFlame)
	{
		psyco2_attack_flag_ = true;
		generate_pshychokinesis(transform_.position() + GSvector3::up() * 2.0f, GSvector3::up() * 3.0f);
	}
}
void SurogSakones::stun(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		if(hp_ <= 0.0f)
		{
			gsPlaySE(SE_FireBolt);
			change_state(State::Dying, MotionAttack3, false);
			return;
		}
		if(is_turn(player_))
		{
			turn();
			return;
		}
		idol(delta_time);		
	}
}
void SurogSakones::dying(float delta_time) {
	world_->particle_manager()->boss_retreat(transform_.position()+GSvector3{0.0f,0.0f,0.3f});
	if (state_timer_ >= mesh_.motion_end_time()) {
		die();
		world_->game_clear();
	}
}
void SurogSakones::turn(float delta_time) {
	if (!flip_)to_rotate_ = GSvector3{ 0.0f, 90.0f ,0.0f };
	else to_rotate_ = { 0.0f, -90.0f ,0.0f };
	if (GSquaternion::angle(transform_.rotation(), GSquaternion::euler(to_rotate_)) >= 5.0f) {
		transform_.rotate(GSvector3{ 0.0f,1.0f,0.0f }, 3.0f * delta_time, GStransform::Space::World);
	}
	if (state_timer_ >= mesh_.motion_end_time()) {
		transform_.rotation(GSquaternion::euler(to_rotate_));
		flip_ = !flip_;
		idol(delta_time);
	}
}
void SurogSakones::move(float delta_time) {
	if (is_turn(player_))
	{
		turn();
		return;
	}
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
	if (target_distance(player_) <= MinMoveDistance ||
		(target_distance(player_) > SarchMoveDistance && target_distance(player_) <= MaxMoveDistance))
	{
		move_way_ = Move::Normal;
	}
	else if (target_distance(player_) > MinMoveDistance && target_distance(player_) <= SarchMoveDistance)
	{
		move_way_ = Move::Normal;
	}
	if (cool_timer_ > 0.0f)move_way_ = Move::Slowly;
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
		vel = GSvector3{ 1.0f,0.0f,0.0f }*MoveSpeed*delta_time;
		transform_.translate(vel, GStransform::Space::World);
	}
	else if (transform_.position() > destination_) {
		vel = GSvector3{ -1.0f,0.0f,0.0f }*MoveSpeed*delta_time;
		transform_.translate(vel, GStransform::Space::World);
	}
}
void SurogSakones::move_slowly(float delta_time)
{
	float distance = target_distance(player_);
	if (is_psyco1_attack(player_))
	{
		psyco1_attack();
		return;
	}
	if (distance <= MinMoveDistance && distance >= ScytheRange)
	{
		if (flip_)
		{
			transform_.translate(GSvector3{ SlowMoveSpeed * 1.1f,0.0f,0.0f }*delta_time, GStransform::Space::World);
			return;
		}
		else
		{
			transform_.translate(GSvector3{ -SlowMoveSpeed * 1.1f,0.0f,0.0f }*delta_time, GStransform::Space::World);
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
#ifdef _DEBUG
	//debug_draw();
	collider().draw();
#endif
}

void SurogSakones::draw_gui() const
{
	if(is_move(player_))
	{
		draw_hp_3D();
	}	
}

void SurogSakones::draw_hp_2D() const
{
	//HPゲージ
	float percent = hp_ / max_hp_;
	GSrect sourceRect{ 0.0f,0.0f,HP_Length * percent,50.0f };
	static const GSvector2 scale{ 1.0f,0.5f };
	GScolor color{ 1.0f,0.0f,0.0f,1.0f };
	if (hp_ >= 0.0f)gsDrawSprite2D(Texture_BossHP, &hp_position, &sourceRect, NULL, &color, &scale, 0.0f);

	//上下の蓋
	GSrect frame_rect{ 0.0f,0.0f,50.0f,52.0f };
	float mag_scale = HP_Length / 50.0f;
	GSvector2 frame_scale{ mag_scale,scale.y };
	gsDrawSprite2D(Texture_BossHPFrame, &hp_position, &frame_rect, NULL, NULL, &frame_scale, 0.0f);

	//左右の蓋描画
	float interval = 5.0f * scale.y;
	GSvector2 framelid_left_position{ hp_position.x - interval,hp_position.y + interval };
	GSvector2 framelid_right_position{ hp_position.x + HP_Length,hp_position.y + interval };
	GSrect frame_lid_rect{ 0.0f,0.0f,5.0f,42.0f };
	GSvector2 frame_lid_scale{ scale.y,scale.y };
	gsDrawSprite2D(Texture_BossHPFrame_Lid, &framelid_left_position, &frame_lid_rect, NULL, NULL, &frame_lid_scale, 0.0f);
	gsDrawSprite2D(Texture_BossHPFrame_Lid, &framelid_right_position, &frame_lid_rect, NULL, NULL, &frame_lid_scale, 0.0f);
}

void SurogSakones::draw_hp_3D() const
{
	//HPゲージ
	float percent = hp_ / max_hp_;
	
	//表示
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GSvector3 position{ transform_.position() + GSvector3{ -0.75f,3.5f,0.0f } };
	GSrect gage_sprite_rect{ 0.0f,0.0f,1.5f * percent,0.1f };
	GSvector3 gage_position{position};
	GScolor color = GScolor{ 1.0f,0.0f,0.0f,1.0f };
	
	if(hp_ >= 0.0f)gsDrawSprite3D(Texture_BossHP,&gage_position,&gage_sprite_rect,NULL,&color,NULL,0.0f);

	GSvector3 lid_lpos{ position + GSvector3{-0.025f / 2.0f,0.0f,0.0f} };
	GSvector3 lid_rpos{ position + GSvector3{1.5f,0.0f,0.0f} };
	static const GSrect lid_lr_spriterect{ 0.0f,0.0f,0.025f,0.1f };
	gsDrawSprite3D(Texture_BossHPFrame_Lid, &lid_lpos, &lid_lr_spriterect, NULL, &color, NULL,0.0f);	
	gsDrawSprite3D(Texture_BossHPFrame_Lid,&lid_rpos,&lid_lr_spriterect,NULL,&color,NULL,0.0f);

	static const GSrect lid_spriterect{ 0.0f,0.0f,1.5f,0.115f };
	GSvector3 lid_pos{ position + GSvector3{0.0f,-0.01f,0.0f} };
	gsDrawSprite3D(Texture_BossHPFrame,&lid_pos,&lid_spriterect,NULL,NULL,NULL,0.0f);

	glPopAttrib();
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
	gsTextPos(0.0f, 160.0f);
	gsDrawText("向いている方向：%d", flip_);
}
void SurogSakones::debug_input()
{
	if (gsGetKeyTrigger(GKEY_1))
	{
		gsPlaySE(SE_GhostDeath);
		change_state(State::Stun, MotionDamage1, false);
		hp_ = -10.0f;
	}
	if (gsGetKeyTrigger(GKEY_2))
	{
		hp_ -= 10.0f;
	}
}

void SurogSakones::scythe_attack()
{
	generate_attackcollider();
	play_se_attack(SE_GhostAttack1);
	gsPlaySE(SE_Slash);
	change_state(State::Attack, MotionScytheAttack, true);
}

void SurogSakones::psyco1_attack()
{
	generate_pshychokinesis(transform_.position() + GSvector3::up() * 3.0f,GSvector3::up() * 4.0f);
	play_se_attack(SE_GhostAttack2);
	change_state(State::Attack, MotionAttack2, false);
}

void SurogSakones::psyco2_attack()
{
	change_state(State::Attack, MotionAttack3, false);
}

void SurogSakones::turn()
{
	gsPlaySE(SE_Slash);
	gsPlaySE(SE_GhostAttack1);
	cool_timer_ = TurnAttackCoolTime;
	generate_attackcollider(true);
	change_state(State::Turn, MotionScytheAttack, true);
}

void SurogSakones::play_se_attack(GSuint se)
{
	++se_attack_counter_;
	if (se_attack_counter_ >= 3) {
		gsPlaySE(se);
		se_attack_counter_ = 0;
	}
}

void SurogSakones::play_se_damage(bool flag)
{
	gsPlaySE(SE_GhostDamage);
	if(!flag)
	{
		++se_attack_counter_;
		if (se_attack_counter_ >= 3) {
			gsPlaySE(SE_BossGhostDamage);
			se_attack_counter_ = 0;
		}
	}
	else
	{
		gsPlaySE(SE_BossGhostDamage);
	}
}


void SurogSakones::generate_pshychokinesis(const GSvector3& position, GSvector3 velocity) {
	if (player_ != nullptr) {
		//プレイヤーのベクトルを求める
		GSvector3 to_player = (player_->transform().position() - transform().position()).normalized();
		//球を出す処理
		world_->add_actor(new PsycokinesisBullet(world_, position, velocity));
	}
}

void SurogSakones::generate_attackcollider(bool is_turn) {
	const float AttackColliderDistance{ 1.3f };
	const float AttackColliderRadius{ 1.0f };
	const float AttackColliderHeight{ 1.85f };

	const float AttackCollideDelay{ 60.0f };
	const float AttackCollideLifeSpan{ 20.0f };
	if(!is_turn)
	{
		GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
		position.y += AttackColliderHeight;
		BoundingSphere collider{ AttackColliderRadius,position };
		world_->add_actor(new AttackCollider{ world_,collider,"EnemyAttack","BossAttack",AttackCollideLifeSpan,AttackCollideDelay });
	}
	else
	{
		GSvector3 Direction;
		if (flip_)Direction = GSvector3{ -1.0f,0.0f,0.0f };
		else Direction = GSvector3{ 1.0f,0.0f,0.0f };
		GSvector3 position = transform_.position() + Direction * AttackColliderDistance;
		position.y += AttackColliderHeight;
		BoundingSphere collider{ AttackColliderRadius,position };
		world_->add_actor(new AttackCollider{ world_,collider,"EnemyAttack","BossAttack",AttackCollideLifeSpan,AttackCollideDelay });
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
	return (target_distance(other) <= ScytheRange && target_signed_angle(other) <= 60.0f && cool_timer_ <= 0.0f);
}
bool SurogSakones::is_psyco1_attack(const Actor* other) {
	if (other == nullptr)return false;
	return (target_distance(other) <= MaxMoveDistance && target_distance(other) >= MinMoveDistance && cool_timer_ <= 0.0f);
}
bool SurogSakones::is_psyco2_attack(const Actor* other) {
	return false;
	//プレイヤーの状態が憑依しているなら
	//Player* player = (Player*)other;
}
bool SurogSakones::is_turn(const Actor* other)
{
	return (flip_!=target_posrelation(other));
}

bool SurogSakones::is_move(const Actor* other) const
{
	return (target_distance(other) <= MaxMoveDistance);
}

bool SurogSakones::is_collide_actor()
{
	return (state_ == State::Idol) || (state_ == State::Move) || (state_==State::Stun);
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

void SurogSakones::collide_actor(Actor& other)
{
	// ｙ座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	position.z = 0.0f;
	GSvector3 target = other.transform().position();
	target.z = 0.0f;
	target.y = 0.0f;
	// 相手との距離
	float distance = GSvector3::distance(position, target);
	// 衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	// 衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	// 重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	collide_field();
}

