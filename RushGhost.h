#ifndef RUSH_GHOST_H_
#define RUSH_GHOST_H_

#include"Actor.h"

class RushGhost : public Actor {
public:
	//���
	enum class State {
		Move,
		Attack,
		Damage,
		Die
	};
	//�R���X�g���N�^
	RushGhost(IWorld* world, const GSvector3& position);
	//�X�V
	void update(float delta_time) override;
	//�`��
	void draw() const override;
	//�Փ˃��A�N�V����
	void react(Actor& other) override;

private:
	//��Ԃ̍X�V
	void update_state(float delta_time);
	//��Ԃ̕ύX
	void change_state(State state, GSuint motion);
	//�ړ�
	void move(float delta_time);
	//�U��
	void attack(float delta_time);
	//�_���[�W
	void damage(float delta_time);
	//����
	void died(float delta_time);

private:
	//�̗�
	float hp_{ 1.0f };
	//�^�C�}�[
	float moving_timer_{ 0.0f};
	//�����X�s�[�h
	float speed_{ 1.0f };
	//���
	State state_;
	//��ԃ^�C�}�[
	float state_timer_;
};


#endif

