#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Actor.h"

class ParticleManager:public Actor
{
public:	
	ParticleManager(IWorld* world);
	void spark(const GSvector3& position);
	
};

#endif