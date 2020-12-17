#pragma once
#include "Player.h"

struct player_ghost : public Player
{
	// プレーヤーの状態
	enum class State {
		Idle,		// 通常
		Attack,		// 攻撃中
		Damage		// ダメージ中
	};
	
	float gravity_timer_{0};
	void on_hit(const Actor& attacker, float atk_power) override;
	player_ghost(IWorld* world, const GSvector3& position);
	void update(float delta) override;
};