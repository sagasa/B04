#pragma once
#include "PlayerInfo.h"

struct Paladin : public PlayerInfo
{
	bool attack() override;
	void draw() override;
	bool hit() override;
	void update(const Player& player, float delta) override;
};

