#pragma once
#include "HPBar.h"
#include "Player.h"

struct player_ghost : public Player
{
	// プレーヤーの状態
	enum State {
		Idle,		// 通常
		Attack,		// 攻撃中
		Damage		// ダメージ中
	};
	HPBar bar_{ 3,120,GScolor{1,0,0,1},GScolor{1,0,0,0.5f} };
	State state_{Idle};
	// 状態タイマ
	float state_timer_{ 0 };
	// モーションのループ指定
	bool motion_loop_{ false };

	// 状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	
	void attack();
	
	float gravity_timer_{0};
	bool on_hit(const Actor& attacker, float atk_power) override;
	player_ghost(IWorld* world, const GSvector3& position,const int hp = 3);
	void update(float delta) override;
	
	void wake_up(const int base_hp) override;
public:
	virtual void draw() const override;
	void stop() override;
};