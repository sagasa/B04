#pragma once

#include "Collision.h"
#include "Actor.h"
class PhysicsActor:public Actor
{
public:
	PhysicsActor();

	//0=ëOÇÃà íu 1=åªç¿ÇÃà íu
	collisions::Box2D get_collision(const float f) const;
	
	bool is_collide(const Actor& other) const override;

	void collide(Actor& other) override;

	bool isHit;
	
	collisions::Box2D box2d;

protected:
	collisions::physics_mat mat;
	GStransform last_transform_;

	void react_physics(const PhysicsActor& pother, const collisions::Box2D& box0, const collisions::Box2D& box1);
};

