#ifndef RUSH_GHOST_H_
#define RUSH_GHOST_H_

#include"Actor.h"
#include"DamageProp.h"
#include"AnimatedMesh.h"

class RushGhost : public Actor ,public DamageProp{
public:
	//���
	enum class State {
		Idle,
		Move,
		Damage,
		Died
	};
	//�R���X�g���N�^
	RushGhost(IWorld* world, const GSvector3& position);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

	//�U�����󂯂�
	virtual bool on_hit(const Actor& attacker, float atk_power) override;

private:
	//��Ԃ̍X�V
	void update_state(float delta_time);
	//��Ԃ̕ύX
	void change_state(State state, GSuint motion,bool loop = true);
	//�A�C�h��
	void idle(float delta_time);
	//�ړ�
	void move(float delta_time);
	//�^�[��
	void turn(float delta_time);
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
	//�ړ�����
	bool is_move()const;

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
	//�^�C�}�[
	float moving_timer_{ 0.0f};
	//���
	State state_;
	//��ԃ^�C�}�[
	float state_timer_;
	//�p�x
	float angle_;
	//�|�C���g
	float point_;
	//��]�̒��S
	GSvector3 rotate_centrer_;
	//�ړ��������
	float to_targe_angle_;
	
};


#endif

