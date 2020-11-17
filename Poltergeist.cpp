#include"Poltergeist.h"
#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
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
//移動判定の距離ｙ
const float AttackDistance_x{ 100.0f };
//移動判定の距離x
const float AttackDistance_y{ 100.0f };
//振り向く角度
const float TurnAngle{ 2.5f };
//エネミーの高さ
const float EnemyHeight{ 0.75f };
//エネミーの半径
const float EnemyRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//重力
const float Gravity{ -0.016f };

//コンストラクタ
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_Poltergeist,Mesh_CarGhost,Mesh_CarGhost,MotionIdle},
	motion_{MotionIdle} ,
	motion_loop_{true},
	state_{ State::Idle },
	state_timer_{0.0f},
	player_{nullptr},
	hp_{1} {
	//ワールドの設定
	world_ = world;
	//名前の設定
	name_ = "Poltergeist";
	//タグ名の設定
	tag_ = "EnemyTag";
	//衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius ,GSvector3{0.0f,EnemyHeight,0.0f} };
	transform_.position(position);
	transform_.localScale(GSvector3{ 0.3f,0.3f,0.3f });
	mesh_.transform(transform_.localToWorldMatrix());
}

//更新
void Poltergeist::update(float delta_time) {
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
	//メッシュを更新
	mesh_.update(delta_time);
	//行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
	//射撃タイマー更新
	shootiong_timer_ -= delta_time;

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
	if (other.tag() == "PlayerAttackTag") {
		hp_--;
		if (hp_ <= 0) {
			//ダメージ状態に変更
			change_state(State::Damage, MotionDamage,false);
		} else {
			//攻撃の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に変更
			change_state(State::Damage, MotionDamage,false);
		}
		return;
	}
	//プレイヤーまたはエネミーに衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//状態の更新
void Poltergeist::update_state(float delta_time) {
	switch (state_) {
	case State::Idle: idle(delta_time); break;
	case State::Turn: turn(delta_time); break;
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
		change_state(State::Attack, MotionAttack);
		return;
	}
}

//ターン
void Poltergeist::turn(float delta_time) {

}

//攻撃
void Poltergeist::attack(float delta_time) {
	//モーション終了後に移動中に遷移
	if (state_timer_ >= mesh_.motion_end_time()) {
		idle(delta_time);
	}
	if (shootiong_timer_ <= 0.0f) {
		GSvector3 target = to_target();
		//world_->add_actor(new PoltergeistBullet{ world_,transform_.position(),target });
		shootiong_timer_ = gsRandf(20.0f, 60.0f);
	}
	

	
}

//ダメージ
void Poltergeist::damage(float delta_time) {
	//モーション終了後にダメージ計算
	if (state_timer_ >= mesh_.motion_end_time()) {
		hp_ -= 1.0f;
		if (hp_ <= 0) {
			//死亡状態に変更
			change_state(State::Died, MotionDie);
		}
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
	return(target_distance() <= TurnDistance) && (target_angle() <= 20.0f);
}

//攻撃判定
bool Poltergeist::is_attack()const {
	//攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
	return (target_distance_x() <= AttackDistance_x) && (target_distance_y() <= AttackDistance_y) && (target_angle() <= 180.0f);
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
	//地面との衝突判定(線分との交差判定)
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset,0.0f };
	GSvector3 intersect;//地面との交点
	if (world_->field()->collide(line, &intersect)) {
		//交差した点からy座標のみ補正する
		position.y = intersect.y;
		transform_.position(position);
	}
}

//アクターとの衝突処理
void Poltergeist::collide_actor(Actor& other) {
	//y座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
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