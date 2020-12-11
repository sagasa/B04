#ifndef POLTERGEIST_BULLET_H_
#define POLTERGEIST_BULLET_H_

#include"Actor.h"
#include"ActorProp.h"

class PoltergeistBullet : public Actor,public ActorProp {
public:

	//�R���X�g���N�^
	PoltergeistBullet(IWorld* world, const GSvector3& position,const GSvector3& velocity);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	//��ʊO���H
	bool is_out_camera() const;

private:
	//���S�^�C�}�[
	float died_timer_;
};


#endif