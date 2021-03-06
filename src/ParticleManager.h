#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Actor.h"
#include <vector>
class Particle;

class ParticleManager:public Actor
{
public:	
	ParticleManager(IWorld* world);
	//火花(テストで作成)
	void spark(const GSvector3& position)const;
	//煙
	void smoke(const GSvector3& position)const;
	//未完成
	void distotion(const GSvector3& position)const;
	//ほこり(着地したとき)
	void dust(const GSvector3& position)const;
	//攻撃に当たった時
	void hit(const GSvector3& position)const;
	//濃ゆい紫色の煙
	void death_smoke(const GSvector3& position,const GSvector3& velocity=GSvector3::zero())const;
	void death_smoke2(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	//細い紫色の煙
	void boss_smoke(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	//憑依時の光
	void possession_light(const GSvector3& position)const;
	//憑依解除時の光
	void possession_release_light(const GSvector3& position)const;

	void ring(const GSvector3 position, GSmatrix4* matrix)const;

	void psyco_bullet_small(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	void psyco_bullet_big(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	void boss_retreat(const GSvector3& position)const;
};

#endif