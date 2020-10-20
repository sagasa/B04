#ifndef RUSH_GHOST_H_
#define RUSH_GHOST_H_

#include"Actor.h"

class RushGhost : public Actor {
public:
	//�R���X�g���N�^
	RushGhost(IWorld* world, const GSvector3& position);
	//�X�V
	void update(float delta_time) override;
	//�`��
	void draw() const override;
	//�Փ˃��A�N�V����
	void react(Actor& other) override;

private:
	//�^�C�}�[
	float moving_timer_{ 0.0f};
	//�����X�s�[�h
	float speed_{ 1.0f };
};


#endif

