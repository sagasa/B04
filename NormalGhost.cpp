#include"NormalGhost.h"
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
};

//振り向き判定の距離
const float TurnDistance{ 1.5f };
//攻撃判定の距離
const float AttackDistance{ 1.5f };
//移動判定の距離ｙ
const float MoveDistance_y{ 5.0f };
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
//スピード
const float Speed{ 0.025f };

//コンストラクタ
NormalGhost::NormalGhost(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_CarGhost,Skeleton_CarGhost,Animation_CarGhost,MotionIdle },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.0f },
	player_{ nullptr },
	hp_{ 1 } {
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
}

//更新
void NormalGhost::update(float delta_time) {
	player_ = world_->find_actor("Player");
}

//描画
void NormalGhost::draw() const {

}

//衝突リアクション
void NormalGhost::react(Actor& other) {

}

//状態の更新
void NormalGhost::update_state(float delta_time) {

}

//状態の変更
void NormalGhost::change_state(State state, GSuint motion, bool loop) {

}

//アイドル中
void NormalGhost::idle(float delta_time) {

}

//移動中
void NormalGhost::move(float delta_time) {

}

//ダメージ中
void NormalGhost::damage(float delta_time) {

}

//死亡中
void NormalGhost::died(float delta_time) {

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