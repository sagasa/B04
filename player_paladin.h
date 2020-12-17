#pragma once
#include "Player.h"

struct player_paladin : public Player
{
	// プレーヤーの状態
	enum class State {
		Stop,		// 待機中
		Idle,		// 通常
		Attack,		// 攻撃中
		Damage		// ダメージ中
	};
	
	void wake_up() override;
	void stop() override;
	
	player_paladin(IWorld* world, const GSvector3& position);
	bool on_hit(const Actor& attacker, float atk_power) override;
	void update(float delta) override;
};