#pragma once
#include "AnimatedMesh.h"
#include "Actor.h"


struct player_info
{
	float jump_height;
	int move_speed;
	int dash_speed;

	float attack_interval;
	float hit_interval;
	float jump_interval;

	player_info(AnimatedMesh mesh):mesh_(mesh){};
	
	virtual  bool attack() = 0;
	virtual  bool hit() = 0;
	virtual  void update(Actor& player,float delta) = 0;//これどうしましょう
	virtual  void draw() = 0;
	
	virtual ~player_info() = default;
protected:
	//アニメーションメッシュ
	AnimatedMesh mesh_;
	//モーション番号
	GSuint motion_;
};
