#pragma once
#include "Player.h"

struct player_paladin : public Player
{
	// �v���[���[�̏��
	enum class State {
		Stop,		// �ҋ@��
		Idle,		// �ʏ�
		Attack,		// �U����
		Damage		// �_���[�W��
	};
	
	void wake_up() override;
	void stop() override;
	
	player_paladin(IWorld* world, const GSvector3& position);
	bool on_hit(const Actor& attacker, float atk_power) override;
	void update(float delta) override;
};