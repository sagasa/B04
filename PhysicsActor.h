#pragma once

#include "Collision.h"
#include "Actor.h"
class PhysicsActor:public Actor
{
public:
	PhysicsActor();

	//0=前の位置 1=現座の位置
	collisions::Box2D get_collision(const float f) const;
	
	bool is_collide(const Actor& other) const override;

	void collide(Actor& other) override;

	bool isHit;
	
	collisions::Box2D box2d;

protected:
	collisions::physics_mat mat;
	GStransform last_transform_;
};

