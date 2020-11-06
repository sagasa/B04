#include"PhysicsActor.h"

#include <iostream>

PhysicsActor::PhysicsActor()
{
	using namespace collisions;
	enable_collider_ = true;
	box2d = Box2D{ Vec2{0,0},Vec2{2,2},0};
}

collisions::Box2D PhysicsActor::get_collision()const
{
	return box2d.transform(transform_.localToWorldMatrix());
}

bool PhysicsActor::is_collide(const Actor& other) const
{
	return false;
}

bool testHit(const collisions::Box2D& box0, const collisions::Box2D& box1)
{
	//std::cout << box0.size.x <<" " << box0.size.y << " " << box1.size.x << " " << box1.size.y << std::endl;
	using namespace collisions;
	return
		box1.isInside(box0.p + box0.size * Vec2{ 0.5,0.5 }) ||
		box1.isInside(box0.p + box0.size * Vec2{ -0.5,0.5 }) ||
		box1.isInside(box0.p + box0.size * Vec2{ -0.5,-0.5 }) ||
		box1.isInside(box0.p + box0.size * Vec2{ 0.5,-0.5 });
}


void PhysicsActor::collide(Actor& other)
{
	try
	{
		auto& pother = dynamic_cast<PhysicsActor&> (other);
		if (testHit(get_collision(), pother.get_collision()))
		{
			isHit = true;
			pother.isHit = true;
			//std::cout << " Hit!! " << std::endl;
		}
	}
	catch (const std::bad_cast&)
	{
		Actor::collide(other);
		std::cout << typeid(other).name() << " test col " << std::endl;
	}
}


