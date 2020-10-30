#include"RushGhost.h"
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
const float AttackDistance{ 1.2f };
//移動判定の距離
const float WalkDistance{ 10.0f };
//振り向く角度
const float TurnAngle{ 2.5f };

//コンストラクタ
RushGhost::RushGhost(IWorld* world, const GSvector3& position):
	mesh_{ Mesh_CarGhost, Skeleton_CarGhost, Animation_CarGhost, MotionIdle },
	motion_{ 0 },
	state_{ State::Move } {
	world_ = world;
	name_ = "RushGhost";
	tag_ = "EnemyTag";
	transform_.position(position);
	player_ = world_->find_actor("Player");
	
}

//更新
void RushGhost::update(float delta_time) {
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
void RushGhost::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glScaled(0.5f, 0.5f, 0.5f);
	mesh_.draw();
	glPopMatrix();
}

//衝突リアクション
void RushGhost::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		change_state(State::Damage, MotionDamage);
	}
}

//状態の更新
void RushGhost::update_state(float delta_time) {
	switch (state_) {
	case State::Move: move(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Died: died(delta_time); break;
	}
	//状態タイマーの更新
	state_timer_ += delta_time;
}

//状態の変更
void RushGhost::change_state(State state, GSuint motion) {
	//モーション変更
	motion_ = motion;
	//状態の変更
	state_ = state;
	//状態タイマー初期化
	state_timer_ = 0.0f;
}

//移動
void RushGhost::move(float delta_time) {
	//プレイヤー情報
	Actor* player = world_->find_actor("Player");
	if (player != nullptr) {//プレイヤーがワールド内にいたら

		GSvector3 to_player = (player->transform().position() - transform_.position()).normalized();
		if (to_player.y < 2) {
			velocity_ = GSvector3{ to_player.x,to_player.y,0.0f };
			//スピードを上げる
			speed_ = 1.5f;
		}
	}
	//transform_.translate(velocity_ * delta_time * speed_, GStransform::Space::World);
}

//攻撃
void RushGhost::attack(float delta_time) {
	//モーション終了後に移動中に遷移
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
	}
}

//ダメージ
void RushGhost::damage(float delta_time) {
	//モーション終了後にダメージ計算
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
void RushGhost::died(float delta_time) {
	//モーション終了後に死亡
	if (state_timer_ >= mesh_.motion_end_time()) {
		die();
	}
	
}

//振り向き判定
bool RushGhost::is_turn()const {
	return(target_distance() <= TurnDistance) && (target_angle() <= 20.0f);
}

//攻撃判定
bool RushGhost::is_attack()const {
	//攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
	return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

//移動判定
bool RushGhost::is_move()const {
	//移動距離かつ前方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
	return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
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