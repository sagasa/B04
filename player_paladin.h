#pragma once
#include "player_info.h"

struct player_paladin : public player_info
{
	player_paladin();
	bool attack() override;
	void draw() override;
	bool hit() override;
	void update(const Actor& player, float delta) override;
};

