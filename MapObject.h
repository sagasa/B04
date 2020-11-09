#pragma once
#include "PhysicsActor.h"
#include "Collision.h"

class MapObject:public PhysicsActor 
{
public:
	MapObject(IWorld* world, const GSvector3& position);
	
};
