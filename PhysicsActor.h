#pragma once

#include "Collision.h"
#include "Actor.h"
class PhysicsActor:public Actor
{
public:
	PhysicsActor();

	collisions::Box2D get_collision() const;
	
	bool is_collide(const Actor& other) const override;

	void collide(Actor& other) override;

	bool isHit;
	
	collisions::Box2D box2d;

	
};

