#ifndef PALADIN_SPAWN_H_
#define PALADIN_SPAWN_H_

#include "Actor.h"
class PaladinSpawn :
    public Actor
{
public:
	PaladinSpawn(IWorld* world, const GSvector3& position);
	void update(float delta_time) override;
	void notice();

private:
	void generate_paladin();

private:
	float generate_timer_{ 0.0f };
	bool notice_{ false };
};

#endif