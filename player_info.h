#pragma once
#include "AnimatedMesh.h"
#include "Actor.h"


struct player_info
{
	bool soft_collide{false};
	float jump_height{1};
	int move_speed{1};
	int dash_speed{1};

	float attack_interval;
	float hit_interval;
	float jump_interval;

	player_info(AnimatedMesh mesh);
	
	virtual  bool attack(const IWorld* world) = 0;
	virtual  bool hit() = 0;
	virtual  void input(Actor& player, GSvector2 input)
	{
			
	}
	virtual  void update(const Actor& player,float delta) = 0;//‚±‚ê‚Ç‚¤‚µ‚Ü‚µ‚å‚¤
	virtual  void draw() = 0;
	
	virtual ~player_info() = default;
protected:
	
	//ƒ‚[ƒVƒ‡ƒ“”Ô†
	GSuint motion_;
};
