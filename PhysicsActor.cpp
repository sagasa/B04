#include"PhysicsActor.h"

#include <iostream>


PhysicsActor::PhysicsActor()
{
	using namespace collisions;
	collisions::physics_mat mat;
	enable_collider_ = true;
	box2d = Box2D{ Vec2{0,0},Vec2{2,2},mat };
}

collisions::Box2D PhysicsActor::get_collision(const float f)const
{
	return box2d.transform(last_transform_.localToWorldMatrix().lerp(transform_.localToWorldMatrix(), f));
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
		if (testHit(get_collision(1), pother.get_collision(1)))
		{
			//ÚG‚µ‚½“_‚ğZo ‚à‚¤–Ê“|‚¾‚©‚çƒSƒŠ‰Ÿ‚·
			float f = 0.5f;
			float change = 0.25f;
			for (int i = 0; i < 5; ++i)
			{
				if (testHit(get_collision(f), pother.get_collision(f)))
					f -= change;
				else
					f += change;
				change /= 2;
			}
			transform_.position(last_transform_.position().lerp(transform_.position(), f)) ;

			float friction = box2d.mat.friction * pother.box2d.mat.friction;
			const float restitution = box2d.mat.restitution * pother.box2d.mat.restitution;

			velocity_ *= -1;
			pother.velocity_ *= -1;
			
			isHit = true;
			pother.isHit = true;
			std::cout << " Hit!! "<< tag_<<" " << last_transform_.position().x<<" "<< transform_.position().x <<std::endl;
		}
	}
	catch (const std::bad_cast&)
	{
		Actor::collide(other);
		std::cout << typeid(other).name() << " test col " << std::endl;
	}
}


