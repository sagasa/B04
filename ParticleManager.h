#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Actor.h"
#include <vector>
class Particle;

class ParticleManager:public Actor
{
public:	
	ParticleManager(IWorld* world);
	void spark(const GSvector3& position)const;
	void smoke(const GSvector3& position)const;
	void distotion(const GSvector3& position)const;
	void dust(const GSvector3& position)const;
	void hit(const GSvector3& position)const;
	void death_smoke(const GSvector3& position)const;
	void boss_smoke(const GSvector3& position)const;
	void possession_light(const GSvector3& position)const;
};

#endif