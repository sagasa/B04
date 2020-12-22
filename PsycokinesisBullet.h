#ifndef PSYCOKINESIS_BULLET_H_
#define PSYCOKINESIS_BULLET_H_

#include "Actor.h"
#include "DamageProp.h"

class PsycokinesisBullet :
	public Actor,public DamageProp
{
public:
	PsycokinesisBullet(IWorld* world_, const GSvector3& position, const GSvector3& velocity,float period=3.0f,float power=1.0f);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other) override;
	virtual bool on_hit(const Actor& attacker, float atk_power) override;

private:
	GSvector3 acceleration_;
	float period_;
	float damage_;
};

#endif