#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Actor.h"
#include <vector>
class Particle;

class ParticleManager:public Actor
{
public:	
	ParticleManager(IWorld* world);
	void spark(const GSvector3& position);
	void smoke(const GSvector3& position);
	void distotion(const GSvector3& position);
};

#endif