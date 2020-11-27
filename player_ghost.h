#pragma once
#include "player_info.h"

struct player_ghost : public player_info
{
	
	player_ghost();
	bool attack() override;
	void draw() override;
	bool hit() override;
	void update(Actor& player, float delta) override;
};
