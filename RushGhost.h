#ifndef RUSH_GHOST_H_
#define RUSH_GHOST_H_

#include"Actor.h"
#include"AnimatedMesh.h"

class RushGhost : public Actor {
public:
	//状態
	enum class State {
		Idle,
		Patrol,
		Move,
		Attack,
		Damage,
		Died
	};
	//コンストラクタ
	RushGhost(IWorld* world, const GSvector3& position);
	//更新
	void update(float delta_time) override;
	//描画
	void draw() const override;
	//衝突リアクション
	void react(Actor& other) override;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion);
	//アイドル
	void idle(float delta_time);
	//巡回
	void patrol(float delta_time);
	//移動
	void move(float delta_time);
	//攻撃
	void attack(float delta_time);
	//ダメージ
	void damage(float delta_time);
	//死亡
	void died(float delta_time);

	//振り向き判定
	bool is_turn()const;
	//攻撃判定
	bool is_attack()const;
	//移動判定
	bool is_move()const;

	//ターゲット方向の角度を求める(符号付き)
	float target_signed_angle()const;
	//ターゲット方向の角度を求める
	float target_angle()const;
	//ターゲットの距離を求める
	float target_distance()const;

private:
	//アニメーションメッシュ
	AnimatedMesh mesh_;
	//モーション番号
	GSuint motion_;
	//プレイヤー
	Actor* player_;
	//体力
	float hp_{ 1.0f };
	//タイマー
	float moving_timer_{ 0.0f};
	//動くスピード
	float speed_{ 1.0f };
	//状態
	State state_;
	//状態タイマー
	float state_timer_;
};


#endif

