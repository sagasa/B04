#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Actor.h"
#include <vector>
class Particle;

class ParticleManager:public Actor
{
public:	
	ParticleManager(IWorld* world);
	//�Ή�(�e�X�g�ō쐬)
	void spark(const GSvector3& position)const;
	//��
	void smoke(const GSvector3& position)const;
	//������
	void distotion(const GSvector3& position)const;
	//�ق���(���n�����Ƃ�)
	void dust(const GSvector3& position)const;
	//�U���ɓ���������
	void hit(const GSvector3& position)const;
	//�Z�䂢���F�̉�
	void death_smoke(const GSvector3& position,const GSvector3& velocity=GSvector3::zero())const;
	void death_smoke2(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	//�ׂ����F�̉�
	void boss_smoke(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	//�߈ˎ��̌�
	void possession_light(const GSvector3& position)const;
	//�߈ˉ������̌�
	void possession_release_light(const GSvector3& position)const;

	void ring(const GSvector3 position, GSmatrix4* matrix)const;

	void psyco_bullet_small(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	void psyco_bullet_big(const GSvector3& position, const GSvector3& velocity = GSvector3::zero())const;
	void boss_retreat(const GSvector3& position)const;
};

#endif