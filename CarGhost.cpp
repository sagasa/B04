#include"CarGhost.h"
#include"IWorld.h"
#include"Assets.h"

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4
};

//振り向き判定の距離
const float TurnDistance{ 1.5f };
//攻撃判定の距離
const float AttackDistance{ 15.0f };
//移動判定の距離ｙ
const float AttackDistance_y{ 50.0f };
//振り向く角度
const float TurnAngle{ 2.5f };
//エネミーの高さ
const float EnemyHeight{ 90.0f };
//エネミーの半径
const float EnemyRadius{ 45.0f };

//コンストラクタ
CarGhost::CarGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_CarGhost,Mesh_CarGhost,Mesh_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	state_{ State::Idle } {
	world_ = world;
	name_ = "CarGhost";
	tag_ = "EnemyTag";
	player_ = world_->find_actor("Player");
	//transform_.position(GSvector3::zero());
	mesh_.transform(transform_.localToWorldMatrix());
	collider_ = BoundingSphere{ EnemyRadius ,mesh_.bone_matrices(3).position()};
	transform_.position(position);
	transform_.localScale(GSvector3{0.3f,0.3f,0.3f});
	
}

//更新
void CarGhost::update(float delta_time) {
	//状態の更新
	update_state(delta_time);
	//モーション変更
	mesh_.change_motion(motion_);
	//メッシュの更新
	mesh_.update(delta_time);
	//行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

//描画
void CarGhost::draw() const {
	mesh_.draw();
	collider().draw();
}

//衝突リアクション
void CarGhost::react(Actor& other) {
	if (state_ == State::Damage)return;
	if (other.tag() == "PlayerTag") {
		change_state(State::Damage, MotionDamage);
	}
}

//状態の更新
void CarGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Patrol: patrol(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//状態タイマー更新
	state_timer_ += delta_time;
}

//状態の変更
void CarGhost::change_state(State state, GSuint motion) {
	//モーション変更
	motion_ = motion;
	//状態遷移
	state_ = state;
	//状態タイマーを初期化
	state_timer_ = 0.0f;
}

//アイドル
void CarGhost::idle(float delta_time) {
	//攻撃するか？
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	//プレイヤーを見つけたか？
	if (is_move()) {
		change_state(State::Move, MotionIdle);
		return;
	}
	//振り向くか?
	if (is_turn()) {
		//左に向くか？右に向くか？
		GSuint motion = (target_signed_angle() >= 0.0f) ? MotionIdle : MotionIdle;
		//振り向き状態に遷移
		change_state(State::Turn, motion);
		return;
	}
}

//巡回
void CarGhost::patrol(float delta_time) {
	//攻撃するか？
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	//プレイヤーを見つけたか？
	if (is_move()) {
		change_state(State::Move, MotionIdle);
		return;
	}

}

//移動
void CarGhost::move(float delta_time) {
	//攻撃するか？
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	velocity_ = GSvector3{ to_target().x,to_target().y,0.0f };
	//スピードを上げる
	speed_ = 0.25f;
	//ターゲット方向の角度を求める
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	//ターゲット方向を向く
	transform_.rotate(0.0f, angle, 0.0f);
	//移動
	transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
	
}

//ターン
void CarGhost::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//振り向きモーションが終了したらアイドル中に遷移
		//idle(delta_time);
	}
	else {
		//振り向きモーションをしながらターゲット方向を向く
		float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.0f, angle, 0.0f);
	}
}

//攻撃
void CarGhost::attack(float delta_time) {
	//モーションが終了したらMove状態に遷移
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
}

//ダメージ
void CarGhost::damage(float delta_time) {
	//モーションが終了したらダメージ計算
	if (state_timer_ >= mesh_.motion_end_time()) {
		//プレイヤーのatkを引く
		hp_ -= 1.0f;
		if (hp_ <= 0) {
			//Dieに状態変更
			change_state(State::Died, MotionDie);
		}
	}

}

//死亡
void CarGhost::died(float delta_time) {
	//モーションが終了したら死亡
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//振り向き判定
bool CarGhost::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() <= 20.0f);
}

//攻撃判定
bool CarGhost::is_attack()const {
	//攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が180.0度以下か？
	return (target_distance() <= AttackDistance) && (target_angle() <= 180.0f);
}

//移動判定
bool CarGhost::is_move()const {
	//移動距離かつ前方向のベクトルとターゲット方向のベクトルの角度差が180.0度以下か？
	return (target_distance_y() <= AttackDistance_y) && (target_angle() <= 180.0f);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める(符号付き)
float CarGhost::target_signed_angle()const {
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//前向き方向のベクトルを取得
	GSvector3 forward = transform_.forward();
	//
	return GSvector3::signed_angle(forward, to_target);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める(符号なし)
float CarGhost::target_angle()const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float CarGhost::target_distance()const {
	return (player_->transform().position() - transform_.position()).magnitude();
}

//ターゲット方向のベクトルを求める
GSvector3 CarGhost::to_target() const {
	return (player_->transform().position() - transform_.position()).normalized();
}

//ターゲットとのxの距離を求める
float CarGhost::target_distance_x() const {
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//ターゲットとのyの距離を求める
float CarGhost::target_distance_y() const {
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}