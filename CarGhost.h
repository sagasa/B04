#ifndef CAR_GHOST_H_
#define CAR_GHOST_H_

#include"Actor.h"
#include"AnimatedMesh.h"



class CarGhost : public Actor {
public:
	//���
	enum class State {
		Move,
		Attack,
		Turn,
		Damage,
		Died
	};
	//�R���X�g���N�^
	CarGhost(IWorld* world,const GSvector3& position);
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
	//�U�����
	void turn(float delta_time);
	//�U��
	void attack(float delta_time);
	//�_���[�W
	void damage(float delta_time);
	//���S
	void died(float delta_time);

	//�U���������
	bool is_turn()const;
	//�U������
	bool is_attack()const;
	//�ړ�����
	bool is_move()const;

	//�^�[�Q�b�g�����̊p�x�����߂�(�����t��)
	float target_signed_angle()const;
	//�^�[�Q�b�g�����̊p�x�����߂�
	float target_angle()const;
	//�^�[�Q�b�g�̋��������߂�
	float target_distance()const;

private:
	//�A�j���[�V�������b�V��
	AnimatedMesh mesh_;
	//���[�V�����ԍ�
	GSuint motion_;
	//�v���C���[
	Actor* player_;
	//�̗�
	float hp_{ 1.0f };
	//�^�C�}�[
	float moving_timer_{ 0.0f };
	//�����X�s�[�h
	float speed_{ 1.0f };
	//���
	State state_;
	//��ԃ^�C�}�[
	float state_timer_;
};

#endif