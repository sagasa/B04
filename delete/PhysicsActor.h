#pragma once

#include "Collision.h"
#include "Actor.h"
class PhysicsActor:public Actor
{
public:
	PhysicsActor();

	//0=�O�̈ʒu 1=�����̈ʒu
	collisions::Box2D get_collision(const float f) const;
	
	bool is_collide(const Actor& other) const override;

	void collide(Actor& other) override;

	bool isHit;
	
	collisions::Box2D box2d;

protected:
	collisions::physics_mat mat;

	void react_physics(const PhysicsActor& pother, const collisions::Box2D& box0, const collisions::Box2D& box1);

private:
	//���莞�ɗ��p
	GStransform last_transform_;
	//���莞�ɗ��p
	GSvector3 last_velocity_;

	void store_last()
	{
		last_transform_ = transform_;
		last_velocity_ = velocity_;
	}
};

