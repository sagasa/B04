#ifndef RUSH_GHOST_H_
#define RUSH_GHOST_H_

#include"Actor.h"

class RushGhost : public Actor {
public:
	//状態
	enum class State {
		Move,
		Attack,
		Damage,
		Die
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
	//移動
	void move(float delta_time);
	//攻撃
	void attack(float delta_time);
	//ダメージ
	void damage(float delta_time);
	//死ぬ
	void died(float delta_time);

private:
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

