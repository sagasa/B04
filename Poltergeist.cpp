#include"Poltergeist.h"
#include"PoltergeistBullet.h"
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
//移動判定の距離ｙ
const float AttackDistance_x{ 100.0f };
//移動判定の距離x
const float AttackDistance_y{ 100.0f };
//振り向く角度
const float TurnAngle{ 2.5f };
//エネミーの高さ
const float EnemyHeight{ 1.0f };
//エネミーの半径
const float EnemyRadius{ 30.0f };


//コンストラクタ
Poltergeist::Poltergeist(IWorld* world, const GSvector3& position) :
	mesh_{Mesh_Poltergeist,Skeleton_CarGhost,Animation_CarGhost,MotionIdle},
	motion_{MotionIdle} ,
	state_{ State::Idle } {
	world_ = world;
	name_ = "Poltergeist";
	tag_ = "EnemyTag";
	player_ = world_->find_actor("Player");
	transform_.position(GSvector3::zero());
	collider_ = BoundingSphere{ EnemyRadius,mesh_.bone_matrices(0).position()};
	mesh_.transform(transform_.localToWorldMatrix());
	transform_.position(position);
	transform_.localScale(GSvector3{ 0.5f,0.5f,0.5f });
	transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-45.0f,0.0f }));
	
}

//更新
void Poltergeist::update(float delta_time) {
	//状態の更新
	update_state(delta_time);
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
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	mesh_.draw();
	glPopMatrix();
	collider().draw();
}

//衝突リアクション
void Poltergeist::react(Actor& other) {
	if (other.tag() == "PlayerTag" && hp_ <= 0) {
		//ダメージ状態に変更
		change_state(State::Damage, MotionDamage);
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
void Poltergeist::change_state(State state, GSuint motion) {
	//モーション変更
	motion_ = motion;
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
	return (player_->transform().position() - transform_.position()).magnitude();
}

//ターゲットとのxの距離を求める
float Poltergeist::target_distance_x() const {
	GSvector3 player = player_->transform().position();
	player.y = 0.0f;
	GSvector3 me = transform_.position();
	me.y = 0.0f;
	return (player - me).magnitude();
}

//ターゲットとのyの距離を求める
float Poltergeist::target_distance_y() const {
	GSvector3 player = player_->transform().position();
	player.x = 0.0f;
	GSvector3 me = transform_.position();
	me.x = 0.0f;
	return (player - me).magnitude();
}

//ターゲット方向のベクトルを求める
GSvector3 Poltergeist::to_target() const {
	return (player_->transform().position() - transform_.position()).normalized();
}