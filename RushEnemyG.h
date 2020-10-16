#ifndef RUSH_ENEMY_G_H_
#define RUSH_ENEMY_G_H_

#include"Actor.h"


class RushEnemyG : public Actor {
public:
	//�R���X�g���N�^
	RushEnemyG(IWorld* world,const GSvector3& position);
	//�X�V
	void update(float delta_time) override;
	//�`��
	void draw() const override;
	//�Փ˃��A�N�V����
	void react(Actor& other) override;

private:
	//�^�C�}�[
	float moving_timer_{ 0.0f };
	//�����X�s�[�h
	float speed_{ 1.0f };
};

#endif