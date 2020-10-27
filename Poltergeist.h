#ifndef POLTER_GEIST_H_
#define POLTER_GEIST_H_

#include"Actor.h"

class Poltergeist : public Actor {
public:
	enum class State {
		Move,
		Attack,
		Damage,
		Died
	};
	//�R���X�g���N�^
	Poltergeist(IWorld* world, const GSvector3& position);
	//�X�V
	void update(float delta_time) override;
	//�`��
	void draw() const override;
	//�Փ˃��A�N�V����
	void react(Actor& other) override;

private:
	//��ԍX�V
	void update_state(float delta_time);
	//��ԕύX
	void change_state(State state, GSuint motion);
	//�ړ�
	void move(float delta_time);
	//�U��
	void attack(float delta_time);
	//�_���[�W
	void damage(float delta_time);
	//���S
	void died(float delta_time);

private:
	//�̗�
	float hp_{ 1.0f };
	//�ړ��^�C�}�[
	float moving_timer_{ 0.0f };
	//�ˌ��^�C�}�[
	float shootiong_timer_{ 0.0f };
	//�X�s�[�h
	float speed_{ 1.0f };
	//���
	State state_;
	//��ԃ^�C�}�[
	float state_timer_;
};
#endif