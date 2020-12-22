#pragma once
#include "interact_prop.h"
#include "Player.h"

struct player_paladin : public Player, public interact_prop
{
	// プレーヤーの状態
	enum State {
		Stop,		// 待機中
		Idle,		// 通常
		Move,
		Jump,
		Attack,		// 攻撃中
		Damage		// ダメージ中
	};
	
	State state_{ Idle };

	bool can_interact(const Actor& from) override;
	
	// 状態タイマ
	float state_timer_{ 0 };
	// モーションのループ指定
	bool motion_loop_{ false };

	// 状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	
	void attack();
	
	player_paladin(IWorld* world, const GSvector3& position);
	bool on_hit(const Actor& attacker, float atk_power) override;
	void update(float delta) override;
public:
	void wake_up() override;
	void stop() override;
};