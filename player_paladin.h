#pragma once
#include "HPBar.h"
#include "interact_prop.h"
#include "Player.h"

class PaladinSpawn;

struct player_paladin : public Player, public interact_prop
{
	// プレーヤーの状態
	enum State {
		Wake,		// 起動
		Stop,		// 待機中
		Idle,		// 通常
		Move,
		Jump,
		Attack,		// 攻撃中
		Damage,		// ダメージ中
		Dead		// 消滅待ち
	};

	int sound_timer_{0};
	
	int base_hp_;//本体HP保存

	HPBar base_bar_{ 3,120,GScolor{1,0,0,1},GScolor{1,0,0,0.5f} };
	HPBar bar_{ 3,120,GScolor{0,0,1,1},GScolor{0,0,1,0.5f} };

	GSvector3 camera_pos_;
	
	State state_{ Idle };

	bool can_interact(const Actor& from) override;

	bool attack_se_{false};

	PaladinSpawn* spawn_{ nullptr };
	
	// 状態タイマ
	float state_timer_{ 0 };
	// モーションのループ指定
	bool motion_loop_{ false };

	// 状態の変更
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