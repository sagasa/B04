#include"CarGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"AttackCollider.h"
#include"ActorProp.h"
#include"Camera.h"

enum {
	MotionIdle = 0,
	MotionAttack = 1,
	MotionSpin = 2,
	MotionDamage = 3,
	MotionDie = 4,
	MotionRun = 5,
	MotioinSpawn = 6
};

//振り向き判定の距離
const float TurnDistance{ 5.0f };
//攻撃判定の距離
const float MoveDistance{ 5.0f };
//振り向く角度
const float TurnAngle{ 5.0f };
//エネミーの高さ
const float EnemyHeight{ 0.75f };
//エネミーの半径
const float EnemyRadius{ 0.5f };
//エネミーの横のオフセット
const float EnemySideOffset{ 0.6f };
//足元のオフセット
const float FootOffset{ 0.1f };
//スピード
const float Speed{ 0.05f };
//x座標の死亡座標
const float LimitDistance_x{ 100.0f };
//y座標の死亡座標
const float LimitDistance_y{ 100.0f };
//重力
const float Gravity{ -0.0016f };


//コンストラクタ
CarGhost::CarGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Poltergeist,Skeleton_CarGhost,Animation_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	player_{ nullptr },
	is_turn_{ false },
	is_hit_{ false } {
	//ワールドの設定
	world_ = world;
	//名前の設定
	name_ = "CarGhost";
	//タグ名の設定
	tag_ = "EnemyTag";
	//ActorPropを継承しているか？
	hit_ = true;
	//体力の設定
	hp_ = 1.0f;
	//攻撃力の設定
	atk_power_ = 1.0f;
	//transform_.position(GSvector3::zero());
	//衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	//座標の初期化
	transform_.position(position);
	transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
}

//更新
void CarGhost::update(float delta_time) {
	//カメラの外側にいると何もしない
	if (is_inside()) {
		//プレイヤーを検索
		player_ = world_->find_actor("Player");
		//状態の更新
		update_state(delta_time);
		//重力を更新
		velocity_.y += Gravity * delta_time;
		//重力を加える
		transform_.translate(0.0f, velocity_.y, 0.0f);
		//フィールドとの衝突判定
		collide_field();
		//モーション変更
		mesh_.change_motion(motion_);
		//メッシュの更新
		mesh_.update(delta_time);
		//行列を設定
		mesh_.transform(transform_.localToWorldMatrix());
	}
}

//描画
void CarGhost::draw() const {
	mesh_.draw();

#ifdef _DEBUG
	collider().draw();
	gsFontParameter(GS_FONT_BOLD, 16, "ＭＳ ゴシック");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("CarGhost:ベクトル(%f,%f,%f)", velocity_.x, velocity_.y, velocity_.z);
	gsTextPos(0.0f, 40.0f);
	gsDrawText("CarGhost:座標(%f,%f,%f)", transform_.position().x, transform_.position().y, transform_.position().z);
#endif

}

//衝突リアクション
void CarGhost::react(Actor& other) {
	//ダメージ中または死亡中は何もしない
	if (state_ == State::Damage || state_ == State::Died) return;

	else if (other.tag() == "PlayerTag") {//プレイヤーと衝突した場合
		is_hit_ = true;
		ActorProp::do_attack(other, *this, atk_power_);
	}
}

//攻撃を受ける
void CarGhost::on_hit(const Actor& other, float atk_power) {
	//ダメージ中または死亡中は何もしない
	if (state_ == State::Damage || state_ == State::Died)return;

	if (other.tag() == "PlayerAttack") { //プレイヤーの攻撃と衝突した場合
		hp_ -= atk_power;
		if (hp_ <= 0) {
			//死亡状態に変更
			change_state(State::Died, MotionDie, false);
		}
		else {
			//攻撃の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に変更
			change_state(State::Damage, MotionDamage, false);
		}
		return;
	}
}

//状態の更新
void CarGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Move: move(delta_time); break;
	case State::Turn: turn(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//状態タイマー更新
	state_timer_ += delta_time;
}

//状態の変更
void CarGhost::change_state(State state, GSuint motion, bool loop) {
	//モーション変更
	motion_ = motion;
	//モーションのループ指定
	motion_loop_ = loop;
	//状態遷移
	state_ = state;
	//状態タイマーを初期化
	state_timer_ = 0.0f;
}

//アイドル
void CarGhost::idle(float delta_time) {
	//プレイヤーを見つけたか？
	if (is_move()) {
		change_state(State::Move, MotionRun);
		return;
	}
	//何もなければアイドル
	change_state(State::Idle, MotionIdle);
}

//移動
void CarGhost::move(float delta_time) {
	GSvector3 velocity = GSvector3::zero();
	if (!is_hit_) {
		if (is_turn()) {
			change_state(State::Turn, MotionIdle);
			return;
		}
		velocity = GSvector3{ to_target().x,0.0f,0.0f };
		velocity_ = velocity;
		//ターゲット方向の角度を求める
		float angle = CLAMP(target_signed_angle(), -TurnAngle / 3, TurnAngle / 3);
		//ターゲット方向を向く
		transform_.rotate(0.0f, angle, 0.0f);
	}
	else {
		velocity = GSvector3{ transform_.forward().x,0.0f,0.0f };
		velocity_ = velocity;
		/*GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(velocity_), 12.0f * delta_time);
		transform_.rotation(rotation);*/
	}
	//移動
	transform_.translate(velocity_ * Speed, GStransform::Space::World);

}

//ターン
void CarGhost::turn(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		//振り向きモーションが終了したら移動中に遷移
		change_state(State::Move, MotionRun);
	}
	else {
		//振り向きモーションをしながらターゲット方向を向く
		float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.0f, angle, 0.0f);
		/*GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(-velocity_), 5.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);*/
	}
}

//ダメージ
void CarGhost::damage(float delta_time) {
	if (state_timer_ < mesh_.motion_end_time()) {
		//ノックバック処理
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
		velocity_ -= GSvector3{ velocity_.x,velocity_.y,0.0f } *0.5f * delta_time;
	}
	//モーションが終了したらアイドルへ
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Move, MotionRun);
	}
}

//死亡
void CarGhost::died(float delta_time) {
	//モーションが終了したら死亡
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//移動判定
bool CarGhost::is_move()const {
	//移動距離かつ前方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
	return (target_distance() <= MoveDistance) && (target_angle() <= 20.0f);
}

//振り向き判定
bool CarGhost::is_turn() const {
	//振り向き距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20度以下か？
	return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

//カメラの内側にいるか
bool CarGhost::is_inside() const {
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
float CarGhost::target_signed_angle()const {
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
float CarGhost::target_angle()const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float CarGhost::target_distance()const {
	if (player_ == nullptr)return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//ターゲットとのxの距離を求める
float CarGhost::target_distance_x() const {
	//ターゲットがいなければ最大値を返す
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//ターゲットとのyの距離を求める
float CarGhost::target_distance_y() const {
	//ターゲットがいなければ最大値を返す
	if (player_ == nullptr)return FLT_MAX;
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//ターゲット方向のベクトルを求める
GSvector3 CarGhost::to_target() const {
	//ターゲットがいなければ0を返す
	if (player_ == nullptr)return GSvector3::zero();
	GSvector3 player = player_->transform().position();
	return (player - transform_.position()).normalized();
}

//フィールドとの衝突処理
void CarGhost::collide_field() {
	//壁との衝突判定(球体との判定)
	GSvector3 center;//衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		center.y = transform_.position().y;
		center.z = 0.0f;
		//補正後の座標に変更する
		transform_.position(center);
	}
	//地面との衝突判定(線分との交差判定)
	GSvector3 position2 = transform_.position();
	Line line2;
	line2.start = position2 + collider_.center;
	line2.end = position2 + GSvector3{ 0.0f,-FootOffset,0.0f };
	GSvector3 intersect2;//地面との交点
	if (world_->field()->collide(line2, &intersect2)) {
		//交点した点からy座標のみ補正する
		position2.y = intersect2.y;
		position2.z = 0.0f;
		//座標を変更する
		transform_.position(position2);
		//重力を加える
		velocity_.y = 0.0f;
	}
	if (is_hit_) {
		Line line;
		line.start = collider().center;
		line.end = collider().center + GSvector3{ -EnemySideOffset,0.0f,0.0f };
		GSvector3 intersect;
		GSvector3 position = transform_.position();
		//左側を調べる
		if (world_->field()->collide(line, &intersect)) {
			change_state(State::Turn, MotionIdle);
			position.x += 0.1f;
			position.y = transform_.position().y;
			position.z = 0.0f;
			transform_.position(position);
		}
		//右側を調べる
		line.start = collider().center;
		line.end = collider().center + GSvector3{ EnemySideOffset,0.0f,0.0f };
		if (world_->field()->collide(line, &intersect)) {
			change_state(State::Turn, MotionIdle);
			position.x -= 0.1f;
			position.y = transform_.position().y;
			position.z = 0.0f;
			transform_.position(position);
		}
	}

}

//アクターとの衝突処理
void CarGhost::collide_actor(Actor& other) {
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
