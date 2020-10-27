#ifndef SUROG_SAKONES
#define SUROG_SAKONES

class IWorld;

#include "Actor.h"
#include<vector>
#include"AnimatedMesh.h"

class SurogSakones :
	public Actor
{
public:
	enum class State {
		Unkown,
		Appear,
		Idol,
		ScytheAttack,
		PsycoAttack_1,
		PsycoAttack_2,
		Turn,
		Stun,
		Die,
	};
public:
	SurogSakones(IWorld* world, const GSvector3& position);
	virtual void update(float delta_time)override;
	virtual void late_update(float delta_time)override;
	virtual void draw()const override;
	void Damage();

private:
	void appear_update(float delta_time);
	void normal_update(float delta_time);
	void angry_update(float delta_time);
	void stun_update(float delta_time);
	void die_update(float delta_time);
	void turn_update(float delta_time);
	//��ԕω�
	void change_state(State state, GSuint motion);
	//�O���U��
	void pshychokinesis(const GSvector3& position);
	//���ʂ̈ړ�(���Α��Ɉړ�����)
	void move(float delta_time);
	//�ړ����U��
	void move_attack(float delta_time);
	//�ړ�������̃^�[��
	void turn(float delta_time, float slow_value, bool flip);
	void debug_draw()const;

private:
	State state_ = State::Unkown;
	float hp_{ 0.0f };
	std::vector<GSvector3> move_pos_;
	GSvector3 destination_;
	//�A�j���[�V��������
	AnimatedMesh mesh_;
	//�A�j���[�V����
	GSuint motion_;
	//
	GSvector3 to_rotate_;
	//�����Ă������
	bool flip{ false };
	bool current_flip_{ false };
	
	float state_timer_{ 0.0f };
};

#endif