#pragma once
#include "Player.h"

struct player_paladin : public Player
{
	player_paladin(IWorld* world, const GSvector3& position);
	void on_hit(const Actor& attacker, float atk_power) override;
	void update(float delta) override;
};