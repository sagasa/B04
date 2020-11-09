#pragma once
#include"PhysicsActor.h"

class TestObj : public PhysicsActor 
{
	void change();
	GSvector3 color;
	
public:
	TestObj(IWorld* world, const GSvector3& position,int id);
	
	void draw() const override;

	
	void update(float delta_time) override;
};

