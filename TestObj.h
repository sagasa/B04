#pragma once
#include"PhysicsActor.h"

class TestObj : public PhysicsActor 
{
	void change();

	
public:
	TestObj(IWorld* world, const GSvector3& position);
	
	void draw() const override;

	
	void update(float delta_time) override;
};

