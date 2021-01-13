#include"Poltergeist.h"
#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"DamageProp.h"
#include"Camera.h"
#include<iostream>

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4
};

//振り向き判定の距離
const float TurnDistance{ 10.0f };
//移動判定の距離ｙ
const float MoveDistance{ 10.0f };
//振り向く角度
const float TurnAngle{ 5.0f };
//攻撃する角度
const float AttackAngle{ 90.0f };
//エネミーの高さ
const float EnemyHeight{ 0.75f };
//エネミーの半径
const float EnemyRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//射撃時間のインターバル
const float Interval{240.0f};
//x座標の死亡座標
const float LimitDistance_x{ 100.0f };
//y座標の死亡座標
const float LimitDistance_y{ 100.0f };
//攻撃力の設定
const float atk_power = 1.0f;

//コンストラクタ
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_Poltergeist,Skeleton_Poltergeist,Animation_Poltergeist,MotionIdle},
	motion_{MotionIdle} ,
	motion_loop_{true},
	state_{ State::Idle },
	state_timer_{0.0f},
	player_{nullptr},
	shootiong_timer_{ 0.0f }{
	//ワールドの設定
	world_ = world;
	//名前の設定
	name_ = "Poltergeist";
	//タグ名の設定
	tag_ = "EnemyTag";
	//体力の設定
	hp_ = 1.0f;
	//衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	mesh_.transform(transform_.localToWorldMatrix());
}

//更新
void Poltergeist::update(float delta_time) {
	if (transform_.position().x <= 0.0f) {
		change_state(State::Died, MotionDie, false);
	}
	//カメラの外側にいると何もしない
	if (is_inside()) {
		//プレイヤーを検索
		player_ = world_->find_actor("Player");
		if (player_ == nullptr) {
			player_ = world_->find_actor("PlayerPaladin");
			if (player_ == nullptr) return;
		}
		//状態の更新
		update_state(delta_time);
		//フィールドとの衝突判定
		collide_field();
		//モーション変更
		mesh_.change_motion(motion_);
		//メッシュを更新
		mesh_.update(delta_time);
		//行列を設定
		mesh_.transform(transform_.localToWorldMatrix());
		//射撃タイマー更新
		shootiong_timer_ -= delta_time;
	}
}

//描画
void Poltergeist::draw() const {
	mesh_.draw();
	collider().draw();
}

//衝突リアクション
void Poltergeist::react(Actor& other) {
	//ダメージ中または死亡中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Died) return;

}

bool Poltergeist::on_hit(const Actor& other, float atk_power) {
	//ダメージ中または死亡中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Died) return false;

	if (other.tag() == "PlayerAttack") {
		hp_ -= atk_power;
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
		return true;
	}
	return false;
}

//状態の更新
void Poltergeist::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Found: found(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//状態タイマー更新
	state_timer_ += delta_time;
}

//状態の変更
void Poltergeist::change_state(State state, GSuint motion,bool loop) {
	//モーション変更
	motion_ = motion;
	//モーションのループ指定
	motion_loop_ = loop;
	//状態の変更
	state_ = state;
	//状態タイマーの初期化
	state_timer_ = 0.0f;
}

//アイドル
void Poltergeist::idle(float delta_time) {
	//攻撃するか？
	if (is_attack()) {
		change_state(State::Found, MotionIdle);
		return;
	}
	//何もなければアイドルへ
	change_state(State::Idle, MotionIdle);
}

//ターン
void Poltergeist::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//振り向きモーションが終了したら発見中に遷移
		found(delta_time);
	}
	else {
		//振り向きモーションをしながらターゲット方向を向く
		float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.0f, angle, 0.0f);
	}
}

//プレイヤーを見つけた
void Poltergeist::found(float delta_time) {
	//振り向くか?
	if (is_turn()) {
		change_state(State::Turn, MotionIdle);
		return;
	}
	//攻撃するか？
	if (is_attack() && shootiong_timer_ <= 0.0f) {
		change_state(State::Attack, MotionAttack);
		return;
	}
}

//攻撃
void Poltergeist::attack(float delta_time) {
	//モーション終了後に移動中に遷移
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
	else if (state_timer_ >= mesh_.motion_end_time() - 30.0f && shootiong_timer_ <= 0.0f) {
		generate_bullet();
		shootiong_timer_ = Interval;
	}
}

//ダメージ
void Poltergeist::damage(float delta_time) {
	//モーション終了後にアイドルへ
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
	
}

//死亡
void Poltergeist::died(float delta_time) {
	//モーション終了後に死亡する
	if (state_timer_ >= mesh_.motion_end_time()-30.0f) {
		die();
	}
	
}

//振り向き判定
bool Poltergeist::is_turn()const {
	return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

//攻撃判定
bool Poltergeist::is_attack()const {
	//攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
	return (target_distance_x() <= MoveDistance) && (target_angle() <= AttackAngle);
}

//カメラの内側にいるか？
bool Poltergeist::is_inside() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//画面内にいたら移動する
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//カメラの前ベクトル
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle <= 45.0f);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める(符号付き)
float Poltergeist::target_signed_angle()const {
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
float Poltergeist::target_angle()const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float Poltergeist::target_distance()const {
	//ターゲットがいなければ最大値を返す
	if (player_ == nullptr) return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//ターゲットとのxの距離を求める
float Poltergeist::target_distance_x() const {
	//ターゲットがいなければ最大値を返す
	if (player_ == nullptr) return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//ターゲットとのyの距離を求める
float Poltergeist::target_distance_y() const {
	//ターゲットがいなければ最大値を返す
	if (player_ == nullptr) return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//ターゲット方向のベクトルを求める
GSvector3 Poltergeist::to_target() const {
	//ターゲットがいなければ0を返す
	if (player_ == nullptr)return GSvector3::zero();
	return (player_->transform().position() - transform_.position()).normalized();
}

//フィールドとの衝突処理
void Poltergeist::collide_field() {
	//壁との衝突判定(球体との判定)
	BoundingSphere sphere{ collider_.radius,transform_.position() };
	GSvector3 center;//衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		//y座標は変更しない
		center.y = transform_.position().y;
		//補正後の座標に変更する
		transform_.position();
	}
}

//アクターとの衝突処理
void Poltergeist::collide_actor(Actor& other) {
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
	v.z = 0.0f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}

//弾の生成
void Poltergeist::generate_bullet() const {
	//弾を生成する場所の距離
	const float GenerateDistance{ 0.75f };
	//弾の生成位置の高さの補正値
	const float GenerateHeight{ 1.0f };
	//弾のスピード
	const float BulletSpeed{ 0.075f };
	//生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	//y座標を補正する
	position.y += GenerateHeight;
	//z座標を0にする
	position.z = 0.0f;
	//移動量の計算
	GSvector3 velocity = to_target() * BulletSpeed;
	//弾の生成
	world_->add_actor(new PoltergeistBullet{ world_,position,velocity, atk_power});
}