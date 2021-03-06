#ifndef SUROG_SAKONES
#define SUROG_SAKONES

class IWorld;

#include "Actor.h"
#include "DamageProp.h"
#include<vector>
#include"AnimatedMesh.h"

class SurogSakones :
	public Actor,public DamageProp
{
public:
	enum class State {
		Unkown,
		Appear,
		Idol,
		Attack,
		Move,
		Turn,
		Stun,
		Dying,
	};
private:
	enum class Move
	{
		Normal,
		Slowly,
		Fast,
	};
public:
	SurogSakones(IWorld* world, const GSvector3& position);	
	virtual void update(float delta_time)override;
	virtual void late_update(float delta_time)override;
	virtual void draw()const override;
	virtual void draw_gui() const override;
	virtual void react(Actor& other)override;
	virtual bool on_hit(const Actor& attacker, float atk_power) override;

private:
	void update_state(float delta_time);
	void appear(float delta_time);
	void idol(float delta_time);
	void attack(float delta_time);
	void scythe_attack(float delta_time);
	void psyco1_attack(float delta_time);
	void psyco2_attack(float delta_time);
	void stun(float delta_time);
	void dying(float delta_time);
	void turn(float delta_time);
	
	void move(float delta_time);
	//普通に近づく
	void move_normal(float delta_time);
	//ゆっくり移動近づきすぎない。(逆に離れるぐらい)
	void move_slowly(float delta_time);
	//早く近づき攻撃
	void move_fast(float delta_time);
	//状態変化
	void change_state(State state, GSuint motion,bool loop=true);
	//念動攻撃
	void generate_pshychokinesis(const GSvector3& position,GSvector3 velocity=GSvector3::up());
	//近接攻撃
	void generate_attackcollider(bool is_turn=false);
	//移動しつつ攻撃
	void move_attack(float delta_time);
	//移動した後のターン
	void turn();
	void flip();
	void debug_draw()const;
	void debug_input();

	void scythe_attack();
	void psyco1_attack();
	void psyco2_attack();

	//計算用関数
	float target_distance(const Actor* other)const;
	float target_signed_angle(const Actor* other)const;
	float target_angle(const Actor* other)const;
	bool target_posrelation(const Actor* other)const;
	
	//判断
	bool is_scythe_attack(const Actor* other);
	bool is_psyco1_attack(const Actor* other);
	bool is_psyco2_attack(const Actor* other);
	bool is_turn(const Actor* other);
	bool is_move(const Actor* other)const;
	bool is_collide_actor();

	void collide_field();
	void collide_actor(Actor& other);

	void draw_hp_2D()const;
	void draw_hp_3D()const;

	void play_se_attack(GSuint se);
	void play_se_damage(bool flag=false);


private:
	State state_ = State::Unkown;
	State prev_state_;
	float state_timer_{ 0.0f };
	//スタン値
	float stun_counter_{ 0.0f };
	int se_damage_counter_{ 0 };
	int se_attack_counter_{ 0 };

	GSvector3 destination_{ GSvector3::zero() };
	
	Move move_way_;
	//アニメーション制御
	AnimatedMesh mesh_;
	//アニメーション
	GSuint motion_;
	GSuint prev_motion_;
	bool loop_{ true };
	//
	GSvector3 to_rotate_;
	//向いている方向,falseなら←、trueなら→
	bool flip_{ false };
	bool prev_flip_{ false };

	bool scythe_attack_flag_{ false };
	bool psyco1_attack_flag_{ false };
	bool psyco2_attack_flag_{ false };
	float attack_timer_{ 0.0f };
	float cool_timer_{ 0.0f };

	//最大体力
	float max_hp_{ 0.0f };
	//SE用のフラグ
	bool play_voice_se_{ false };

	//プレイヤー用の入れ物
	Actor* player_;	
};

#endif