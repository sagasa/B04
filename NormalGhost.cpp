#include"NormalGhost.h"
#include"IWorld.h"
#include"Field.h"
#include"Line.h"
#include"Assets.h"
#include"Camera.h"
#include"DamageProp.h"
#include"ParticleManager.h"
#include"Camera.h"

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
//移動判定の距離ｙ
const float MoveDistance{10.0f};
//振り向く角度
const float TurnAngle{ 5.0f };
//エネミーの高さ
const float EnemyHeight{ 0.75f };
//エネミーの半径
const float EnemyRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//頭上のオフセット
const float HeadOffset{ 1.0f };
//移動速度
const float Speed{ 0.05f };
//x座標の死亡座標
const float LimitDistance_x{ 100.0f };
//y座標の死亡座標
const float LimitDistance_y{ 100.0f };

const float atk_power = 1.0f;

//コンストラクタ
NormalGhost::NormalGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Ghost,Skeleton_Ghost,Animation_Ghost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	player_{ nullptr }{
		//ワールドの設定
		world_ = world;
		//名前の設定
		name_ = "NormalGhost";
		//タグ名
		tag_ = "EnemyTag";
		collider_ = BoundingSphere{ EnemyRadius,GSvector3{0.0f,EnemyHeight,0.0f} };
		//座標の初期化
		transform_.position(position);
		transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
		//メッシュの変換行列を初期化
		mesh_.transform(transform_.localToWorldMatrix());
		hp_ = 1.0f;
		
}

//更新
void NormalGhost::update(float delta_time) {
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
	mesh_.change_motion(motion_);
	//メッシュを更新
	mesh_.update(delta_time);
	//行列を設定
	mesh_.transform(transform_.localToWorldMatrix());

	//重さ軽減のためプレイヤーの座標から-40離れたら死亡
	if (transform_.position().x <= player_->transform().position().x - 20.0f) {
		die();
	}
}

//描画
void NormalGhost::draw() const {
	if (is_inside()) {
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.8f);
		//メッシュの描画
		mesh_.draw();
		glColor3f(1.0f, 1.0f, 1.0f);
		glPopMatrix();
		//衝突判定のデバッグ表示
#ifdef _DEBUG
		collider().draw();
#endif
		if (state_ == State::Died) {
			world_->particle_manager()->death_smoke(transform_.position());
		}
	}
}

//衝突リアクション
void NormalGhost::react(Actor& other) {
	//ダメージ中または死亡中は何もしない
	if (state_ == State::Damage || state_ == State::Died) return;

	if (other.tag() == "PlayerTag") {
		DamageProp::do_attack(other, *this, atk_power);
	}
	
}

bool NormalGhost::on_hit(const Actor& other, float atk_power) {
	//ダメージ中または死亡中は何もしない
	if (state_ == State::Damage || state_ == State::Died) return false;

	if (other.tag() == "PlayerTag" || other.tag() == "PlayerAttack") {
		hp_-= atk_power;
		change_state(State::Damage, MotionDamage, false);
		return true;
	}
	return false;
}

//状態の更新
void NormalGhost::update_state(float delta_time) {
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
void NormalGhost::change_state(State state, GSuint motion, bool loop) {
	//モーション番号の更新
	motion_ = motion;
	//モーションのループ指定
	motion_loop_ = loop;
	//状態の変更
	state_ = state;
	//状態タイマーの初期化
	state_timer_ = 0.0f;
}

//アイドル中
void NormalGhost::idle(float delta_time) {
	if (is_move()) {
		change_state(State::Move, MotionRun);
		return;
	}
	change_state(State::Idle, MotionIdle);
}

//移動中
void NormalGhost::move(float delta_time) {
	if (transform_.position().x <= -10.0f) {
		change_state(State::Died, MotionDie, false);
		return;
	}
	GSvector3 velocity{-1.0f,0.0f,0.0f};
	velocity.z = 0.0f;
	velocity_ = velocity;
	transform_.translate(velocity_ * delta_time * Speed, GStransform::Space::World);
}

//ダメージ中
void NormalGhost::damage(float delta_time) {
	if (state_timer_ >= mesh_.motion_end_time()) {
		change_state(State::Died,MotionDie,false);
	}
}

//死亡中
void NormalGhost::died(float delta_time) {
	//モーション終了後に死亡
	if (state_timer_ >= mesh_.motion_end_time() - 30.0f) {
		die();
	}
}

//移動判定
bool NormalGhost::is_move() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//画面内にいたら移動する
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//カメラの前ベクトル
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle <= 45.0f);
}

//カメラの内側にいるか？
bool NormalGhost::is_inside() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//画面内にいたら移動する
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//カメラの前ベクトル
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle <= 45.0f);
}

//ターゲット方向の角度を求める(符号付)
float NormalGhost::target_signed_angle() const {
	if (player_ == nullptr) return 0.0f;
	GSvector3 to_target = player_->transform().position() - transform_.position();
	GSvector3 forward = transform_.forward();
	return GSvector3::signed_angle(forward,to_target);
}

//ターゲット方向の角度を求める
float NormalGhost::target_angle() const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float NormalGhost::target_distance() const {
	if (player_ == nullptr)return FLT_MAX;
	return (player_->transform().position() - transform_.position()).magnitude();
}

//ターゲット方向のベクトルを求める
GSvector3 NormalGhost::to_target() const {
	if (player_ == nullptr) return GSvector3::zero();
	return (player_->transform().position() - transform_.position()).normalized();
}

//フィールドとの衝突判定
void NormalGhost::collide_field() {
	//壁との衝突判定
	BoundingSphere sphere{ collider().radius,transform().position() };
	GSvector3 center;
	if (world_->field()->collide(sphere, &center)) {
		center.z = 0.0f;
		transform_.position(center);
		change_state(State::Died, MotionDie,false);
	}
	Line line{ collider().center,GSvector3{collider().center.x - EnemyRadius,collider().center.y,0.0f} };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		velocity_.y *= 0.1f;
	}
}

//アクターとの衝突処理
void NormalGhost::collide_actor(Actor& other) {
	//z座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.z = 0.0f;
	GSvector3 target = other.transform().position();
	target.z = 0.0f;
	//相手との距離
	float distance = GSvector3::distance(position,target);
	//衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	v.z = 0.0f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}