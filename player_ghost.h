#pragma once
#include "Player.h"

struct player_ghost : public Player
{
	player_ghost(IWorld* world, const GSvector3& position);
	void update(float delta) override;
};