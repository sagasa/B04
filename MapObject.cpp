#include "MapObject.h"

MapObject::MapObject(IWorld* world, const GSvector3& position)
{
    world_ = world;
    transform_.position(position);
	last_transform_ = transform_;
	using namespace collisions;
	mat.is_static = true;
	box2d = Box2D{ Vec2{0,0},Vec2{40,4},mat };
}

