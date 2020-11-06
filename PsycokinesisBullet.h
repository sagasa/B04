#pragma once
#include "Actor.h"
class PsycokinesisBullet :
	public Actor
{
public:
	PsycokinesisBullet(IWorld* world_, const GSvector3& position, const GSvector3& velocity);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other) override;

private:
	GSvector3 acceleration_;
	float period_;
};

