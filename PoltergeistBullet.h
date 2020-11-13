#ifndef POLTERGEIST_BULLET_H_
#define POLTERGEIST_BULLET_H_

#include"Actor.h"

class PoltergeistBullet : public Actor {
public:

	//�R���X�g���N�^
	PoltergeistBullet(IWorld* world, const GSvector3& position,const GSvector3& target);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	//�v�Z
	void parabola();

private:
	//�^�[�Q�b�g�̈ʒu
	GSvector3 target_;
	GSvector3 this_target_;
	//�I�t�Z�b�g
	GSvector3 offset_;
};


#endif