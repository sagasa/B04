#ifndef POLTER_GEIST_H_
#define POLTER_GEIST_H_

#include"Actor.h"
#include"AnimatedMesh.h"

class Poltergeist : public Actor {
public:
	enum class State {
		Idle,
		Turn,
		Found,
		Attack,
		Damage,
		Died
	};
	//�R���X�g���N�^
	Poltergeist(IWorld* world, const GSvector3& position);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	//��ԍX�V
	void update_state(float delta_time);
	//��ԕύX
	void change_state(State state, GSuint motion,bool loop = true);
	//�A�C�h��
	void idle(float delta_time);
	//�v���C���[��������
	void found(float delta_time);
	//�^�[��
	void turn(float delta_time);
	//�U��
	void attack(float delta_time);
	//�_���[�W
	void damage(float delta_time);
	//���S
	void died(float delta_time);

	//�t�B�[���h�Ƃ̏Փˏ���
	void collide_field();
	//�A�N�^�[�Ƃ̏Փˏ���
	void collide_actor(Actor& other);

	//�U���������
	bool is_turn()const;
	//�U������
	bool is_attack()const;

	//�^�[�Q�b�g�����̊p�x�����߂�(�����t��)
	float target_signed_angle()const;
	//�^�[�Q�b�g�����̊p�x�����߂�
	float target_angle()const;
	//�^�[�Q�b�g�̋��������߂�
	float target_distance()const;
	//�^�[�Q�b�g��x�̋��������߂�
	float target_distance_x() const;
	//target������y�̋��������߂�
	float target_distance_y() const;

	//�^�[�Q�b�g�����̃x�N�g�������߂�
	GSvector3 to_target()const;

private:
	//�A�j���[�V�������b�V��
	AnimatedMesh mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//���[�V�����̃��[�v�w��
	bool motion_loop_;
	//�v���C���[
	Actor* player_;
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