#pragma once
#include "HPBar.h"
#include "interact_prop.h"
#include "Player.h"

class PaladinSpawn;

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
		Damage,		// �_���[�W��
		Dead		// ���ő҂�
	};

	int sound_timer_{0};
	
	int base_hp_;//�{��HP�ۑ�

	HPBar base_bar_{ 3,120,GScolor{1,0,0,1},GScolor{1,0,0,0.5f} };
	HPBar bar_{ 3,120,GScolor{0,0,1,1},GScolor{0,0,1,0.5f} };

	GSvector3 camera_pos_;
	
	State state_{ Idle };

	bool can_interact(const Actor& from) override;

	bool attack_se_{false};

	PaladinSpawn* spawn_{ nullptr };
	
	// ��ԃ^�C�}
	float state_timer_{ 0 };
	// ���[�V�����̃��[�v�w��
	bool motion_loop_{ false };

	// ��Ԃ̕ύX
	void change_state(State state, GSuint motion, bool loop = true) ;
	
	void attack();

	bool is_active()const;
	
	player_paladin(IWorld* world, const GSvector3& position,PaladinSpawn* spawn=nullptr);
	bool on_hit(const Actor& attacker, float atk_power) override;
	void update(float delta) override;
public:
	virtual void draw() const override;
	void wake_up(const int base_hp) override;
	void stop() override;	
};