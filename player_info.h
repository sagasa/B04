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
	virtual  void update(Actor& player,float delta) = 0;//����ǂ����܂��傤
	virtual  void draw() = 0;
	
	virtual ~player_info() = default;
protected:
	//�A�j���[�V�������b�V��
	AnimatedMesh mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
};
