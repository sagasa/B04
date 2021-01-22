#include"RushGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"DamageProp.h"
#include"ParticleManager.h"

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4,
	MotionRun = 5
};

//振り向き判定の距離
const float TurnDistance{ 1.5f };
//移動判定の距離
const float MoveDistance{ 7.0f };
//振り向く角度
const float TurnAngle{ 5.0f };
//エネミーの高さ
const float EnemyHeight{ 0.75f };
//エネミーの半径
const float EnemyRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//円周率
const float PI{ 3.141592654 };
//半径
const float radius{ 5.0f };
//x座標の死亡座標
const float LimitDistance_x{ 100.0f };
//y座標の死亡座標
const float LimitDistance_y{ 100.0f };
//攻撃力の設定
const float atk_power = 1.0f;
//移動中に遷移するまでのインターバル
const float Interval{ 60.0f };

//コンストラクタ
RushGhost::RushGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_RushGhost,Skeleton_RushGhost,Animation_RushGhost, MotionIdle },
	motion_{ MotionIdle },
	state_{ State::Idle },
	angle_{0.0f},
	point_{0.0f} {
	world_ = world;
	name_ = "RushGhost";
	tag_ = "EnemyTag";
	//体力の設定
	hp_ = 3.0f;
	//衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	//座標の初期化
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	//メッシュの変換行列の初期化
	mesh_.transform(transform_.localToWorldMatrix());

}

//更新
void RushGhost::update(float delta_time) {
	//プレイヤーを検索
	player_ = world_->find_actor("Player");
	if (player_ == nullptr) {
		player_ = world_->find_actor("PlayerPaladin");
		if (player_ == nullptr) return;
	}
	//状態の更新
	update_state(delta_time);
	//フィールドとの衝突判定
	//collide_field();
	//モーション変更
	mesh_.change_motion(motion_);
	//メッシュの更新
	mesh_.update(delta_time);
	//プレイヤーの方向へ向く
	turn(delta_time);
	//行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

//描画
void RushGhost::draw() const {
	mesh_.draw();
	collider().draw();
	if (state_ == State::Died) {
		world_->particle_manager()->death_smoke(transform_.position());
	}
}

//衝突リアクション
void RushGhost::react(Actor& other) {
	//ダメージ中または死亡中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Died) return;

	if (other.tag() == "PlayerTag") {
		DamageProp::do_attack(other, *this, atk_power);
	}
}


bool RushGhost::on_hit(const Actor& other, float atk_power) {
	//ダメージ中または死亡中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Died) return false;
	if (other.tag() == "PlayerAttack") {
		hp_ -= atk_power;
		if (hp_ <= 0) {
			//ダメージ状態に変更
			change_state(State::Died, MotionDie, false);
		}
		else {
			//攻撃の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に変更
			change_state(State::Damage, MotionDamage, false);
		}
		return true;
	}
	return false;
}

//状態の更新
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//状態タイマーの更新
	state_timer_ += delta_time;
}

//状態の変更
void RushGhost::change_state(State state, GSuint motion,bool loop) {
	//モーション変更
	motion_ = motion;
	//モーションのループ指定
	motion_loop_ = loop;
	//状態の変更
	state_ = state;
	//状態タイマー初期化
	state_timer_ = 0.0f;
}

//アイドル
void RushGhost::idle(float delta_time) {
	//プレイヤーを見つけてかつ移動タイマーが0になっているか？
	if (is_move() && moving_timer_ <= 0.0f) {
		change_state(State::Move, MotionIdle);
		moving_timer_ = Interval;
		point_ = 0;
		angle_ = 0.0f;
		to_targe_angle_ = (transform_.position().x < player_->transform().position().x) ? -1 : 1;
		rotate_center_ = transform_.position() - to_targe_angle_* GSvector3{ radius,0.0f,0.0f};
		return;
	}
	//移動タイマーを更新
	moving_timer_ -= delta_time;
	change_state(State::Idle, MotionIdle);
}

//移動
void RushGhost::move(float delta_time) {
	//マップ外にいたら死亡
	if (transform_.position().x <= -10.0f) {
		change_state(State::Died, MotionDie, false);
		return;
	}
	//回数
	const int NumOfTimes{ 90};//移動速度が変化
	if (point_ <= NumOfTimes) {//回数
		angle_ = PI * point_ / NumOfTimes;
		GSvector3 position{  rotate_center_.x + to_targe_angle_ *(float)cos(angle_) * radius,rotate_center_.y + -(float)sin(angle_) * radius,0.0f };
		GSvector3 velocity = (position - transform_.position());
		velocity_ = velocity;
		transform_.translate(velocity_, GStransform::Space::World);
		point_ += delta_time;
	}
	else {
		transform_.position(rotate_center_ - to_targe_angle_ * GSvector3{ radius,0.0f,0.0f });
		change_state(State::Idle, MotionIdle);
	}
}

//ターン
void RushGhost::turn(float delta_time) {
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	//ターゲット方向を向く
	transform_.rotate(0.0f, angle, 0.0f);
}

//ダメージ
void RushGhost::damage(float delta_time) {
	//モーション終了後に移動中へ
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}

}

//死亡
void RushGhost::died(float delta_time) {
	//モーション終了後に死亡
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}

}

//振り向き判定
bool RushGhost::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() >= 20.0f);
}

//移動判定
bool RushGhost::is_move()const {
	//移動距離?
	return (target_distance() <= MoveDistance);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める(符号付き)
float RushGhost::target_signed_angle()const {
	//ターゲットがいなければ0を返す
	if (player_ == nullptr)return 0.0f;
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//前向き方向のベクトルを取得
	GSvector3 forward = transform_.forward();
	//
	return GSvector3::signed_angle(forward, to_target);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める(符号なし)
float RushGhost::target_angle()const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float RushGhost::target_distance()const {
	if (player_ == nullptr)return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//ターゲット方向のベクトルを求める
GSvector3 RushGhost::to_target() const {
	//ターゲットがいなければ0を返す
	if (player_ == nullptr)return GSvector3::zero();
	return (player_->transform().position() - transform_.position()).normalized();
}

//ターゲットとのxの距離を求める
float RushGhost::target_distance_x() const {
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//ターゲットとのyの距離を求める
float RushGhost::target_distance_y() const {
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//フィールドとの衝突処理
void RushGhost::collide_field() {
	//壁との衝突判定(球体との判定)
	BoundingSphere sphere{ collider().radius,transform().position() };
	GSvector3 center;//衝突後の球体の中心座標
	if (world_->field()->collide(sphere, &center)) {
		center.z = 0.0f;
		//補正後の座標に変更する
		transform_.position(center);
	}
}

//アクターとの衝突処理
void RushGhost::collide_actor(Actor& other) {
	//z座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.z = 0.0f;
	GSvector3 target = other.transform().position();
	target.z = 0.0f;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}