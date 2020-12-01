#include"RushGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"

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
//攻撃判定の距離
const float AttackDistance{ 1.5f };
//移動判定の距離x
const float MoveDistance_x{ 15.0f };
//移動判定の距離ｙ
const float MoveDistance_y{ 15.0f };
//振り向く角度
const float TurnAngle{ 5.0f };
//エネミーの高さ
const float EnemyHeight{ 0.75f };
//エネミーの半径
const float EnemyRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//スピード
const float Speed{ 0.025f };
//円周率
const float PI{ 3.141592654 };
//半径
const float radius{ 5.0f };

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
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	mesh_.transform(transform_.localToWorldMatrix());

}

//更新
void RushGhost::update(float delta_time) {
	//プレイヤーを検索
	player_ = world_->find_actor("Player");
	//状態の更新
	update_state(delta_time);
	//フィールドとの衝突判定
	collide_field();
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
}

//衝突リアクション
void RushGhost::react(Actor& other) {
	//ダメージ中または死亡中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Died) return;
	if (other.tag() == "PlayerAttackTag") {
		hp_--;
		if (hp_ <= 0) {
			//ダメージ状態に変更
			change_state(State::Damage, MotionDamage, false);
		}
		else {
			//攻撃の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に変更
			change_state(State::Damage, MotionDamage, false);
		}
		return;
	}
	//プレイヤーまたはエネミーに衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//状態の更新
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Attack: attack(delta_time); break;
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
	//攻撃するか？
	if (is_attack()) {
		change_state(State::Attack, MotionAttack);
		return;
	}
	//プレイヤーを見つけたか？
	if (is_move()) {
		change_state(State::Move, MotionIdle);
		point_ = 0;
		angle_ = 0.0f;
		to_targe_angle_ = (transform_.position().x < player_->transform().position().x) ? -1 : 1;
		rotate_centrer_ = transform_.position() - to_targe_angle_* GSvector3{ radius,0.0f,0.0f};
		return;
	}
	change_state(State::Idle, MotionIdle);
}

//移動
void RushGhost::move(float delta_time) {
	//攻撃するか？
	/*if (is_attack()) {
		change_state(State::Attack, MotionAttack);
	}*/
	if (point_ < 100) {//回数
		angle_ = PI * point_ / 100;
		GSvector3 position{  rotate_centrer_.x + to_targe_angle_ *(float)cos(angle_) * radius,rotate_centrer_.y + -(float)sin(angle_) * radius,0.0f };
		GSvector3 velocity = (position - transform_.position());
		velocity_ = velocity;
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
		point_++;
	}
	else {
		change_state(State::Idle, MotionIdle);
	}
	//velocity_ = GSvector3{ to_target().x,to_target().y,0.0f };
	//ターゲット方向の角度を求める
	
	//transform_.translate(velocity_ * delta_time * Speed, GStransform::Space::World);
}

//ターン
void RushGhost::turn(float delta_time) {
	float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
	//ターゲット方向を向く
	transform_.rotate(0.0f, angle, 0.0f);
}

//攻撃
void RushGhost::attack(float delta_time) {
	//モーション終了後に移動中へ
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
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
	if (state_timer_ >= mesh_.motion_end_time()) {
		die();
	}

}

//振り向き判定
bool RushGhost::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() >= 20.0f);
}

//攻撃判定
bool RushGhost::is_attack()const {
	//攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
	return (target_distance() <= AttackDistance);
}

//移動判定
bool RushGhost::is_move()const {
	//移動距離かつ前方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
	return (target_distance_x() <= MoveDistance_x) && (target_distance_y() <= MoveDistance_y) && (target_angle() <= 20.0f);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める(符号付き)
float RushGhost::target_signed_angle()const {
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
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//ターゲットとのyの距離を求める
float RushGhost::target_distance_y() const {
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
	////z座標を除く座標を求める
	//GSvector3 position = transform_.position();
	//position.z = 0.0f;
	//GSvector3 target = other.transform().position();
	//target.z = 0.0f;
	////相手との距離
	//float distance = GSvector3::distance(position, target);
	////衝突判定球の半径同士を加えた長さを求める
	//float length = collider_.radius + other.collider().radius;
	////衝突判定球の重なっている長さを求める
	//float overlap = length - distance;
	////重なっている部分の半分の距離だけ離れる移動量を求める
	//GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	//transform_.translate(v, GStransform::Space::World);
	////フィールドとの衝突判定
	//collide_field();
}