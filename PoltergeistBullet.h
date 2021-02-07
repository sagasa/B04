#ifndef POLTERGEIST_BULLET_H_
#define POLTERGEIST_BULLET_H_

#include"Actor.h"
#include"DamageProp.h"
#include "AnimatedMesh.h"

class PoltergeistBullet : public Actor {
public:

	//�R���X�g���N�^
	PoltergeistBullet(IWorld* world, const GSvector3& position,const GSvector3& velocity, float atk_power = 1.0f);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	//��ʊO���H
	bool is_out_camera() const;
	//�ǂƂ̏Փ˔���
	void collide_field();

private:
	//���S�^�C�}�[
	float died_timer_;
	//�U����
	float atk_power_;

	GSvector3 camera_pos{ 0,0,0 };

	

};
#endif