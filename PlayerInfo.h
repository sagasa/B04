#pragma once

class Player;

struct PlayerInfo
{
	float jump_height;
	int move_speed;
	int dash_speed;

	float attack_interval;
	float hit_interval;
	float jump_interval;
	virtual  bool attack() = 0;
	virtual  bool hit() = 0;
	virtual  void update(const Player& player,float delta) = 0;//‚±‚ê‚Ç‚¤‚µ‚Ü‚µ‚å‚¤
	virtual  void draw() = 0;
};
