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
	void death_smoke(const GSvector3& position)const;
	//�ׂ����F�̉�
	void boss_smoke(const GSvector3& position)const;
	//�߈ˎ��̌�
	void possession_light(const GSvector3& position)const;
	//�߈ˉ������̌�
	void possession_release_light(const GSvector3& position)const;
};

#endif