#ifndef POLTER_GEIST_H_
#define POLTER_GEIST_H_

#include"Actor.h"

class Poltergeist : public Actor {
public:
	enum class State {
		Move,
		Attack,
		Damage,
		Died
	};
	//コンストラクタ
	Poltergeist(IWorld* world, const GSvector3& position);
	//更新
	void update(float delta_time) override;
	//描画
	void draw() const override;
	//衝突リアクション
	void react(Actor& other) override;

private:
	//状態更新
	void update_state(float delta_time);
	//状態変更
	void change_state(State state, GSuint motion);
	//移動
	void move(float delta_time);
	//攻撃
	void attack(float delta_time);
	//ダメージ
	void damage(float delta_time);
	//死亡
	void died(float delta_time);

private:
	//体力
	float hp_{ 1.0f };
	//移動タイマー
	float moving_timer_{ 0.0f };
	//射撃タイマー
	float shootiong_timer_{ 0.0f };
	//スピード
	float speed_{ 1.0f };
	//状態
	State state_;
	//状態タイマー
	float state_timer_;
};
#endif