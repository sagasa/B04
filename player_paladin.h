#pragma once
#include "interact_prop.h"
#include "Player.h"

struct player_paladin : public Player, public interact_prop
{
	// �v���[���[�̏��
	enum State {
		Wake,		// �N��
		Stop,		// �ҋ@��
		Idle,		// �ʏ�
		Move,
		Jump,
		Attack,		// �U����
		Damage		// �_���[�W��
	};

	GSvector3 camera_pos_;
	
	State state_{ Idle };

	bool can_interact(const Actor& from) override;
	
	// ��ԃ^�C�}
	float state_timer_{ 0 };
	// ���[�V�����̃��[�v�w��
	bool motion_loop_{ false };

	// ��Ԃ̕ύX
	void change_state(State state, GSuint motion, bool loop = true);
	
	void attack();
	
	player_paladin(IWorld* world, const GSvector3& position);
	bool on_hit(const Actor& attacker, float atk_power) override;
	void update(float delta) override;
public:
	virtual void draw() const override;
	void wake_up() override;
	void stop() override;
};