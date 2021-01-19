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
	void Damage();

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
	//���ʂɋ߂Â�
	void move_normal(float delta_time);
	//�������ړ��߂Â������Ȃ��B(�t�ɗ���邮�炢)
	void move_slowly(float delta_time);
	//�����߂Â��U��
	void move_fast(float delta_time);
	//��ԕω�
	void change_state(State state, GSuint motion,bool loop=true);
	//�O���U��
	void generate_pshychokinesis(const GSvector3& position,GSvector3 velocity=GSvector3::up());
	//�ߐڍU��
	void generate_attackcollider();
	//�ړ����U��
	void move_attack(float delta_time);
	//�ړ�������̃^�[��
	void turn(float delta_time, float slow_value, bool flip);
	void debug_draw()const;

	void scythe_attack();
	void psyco1_attack();
	void psyco2_attack();

	//�v�Z�p�֐�
	float target_distance(const Actor* other)const;
	float target_signed_angle(const Actor* other)const;
	float target_angle(const Actor* other)const;
	bool target_posrelation(const Actor* other)const;
	
	//���f
	bool is_scythe_attack(const Actor* other);
	bool is_psyco1_attack(const Actor* other);
	bool is_psyco2_attack(const Actor* other);
	bool is_turn(const Actor* other);
	bool is_move(const Actor* other);

	void collide_field();
	void collide_actor(Actor& other);


private:
	State state_ = State::Unkown;
	State prev_state_;
	float state_timer_{ 0.0f };
	//�X�^���l
	float stun_{ 0.0f };
	
	std::vector<GSvector3> move_pos_;
	GSvector3 destination_;
	Move move_way_;
	//�A�j���[�V��������
	AnimatedMesh mesh_;
	//�A�j���[�V����
	GSuint motion_;
	GSuint prev_motion_;
	bool loop_{ true };
	//
	GSvector3 to_rotate_;
	//�����Ă������
	bool flip_{ false };
	bool prev_flip_{ false };

	bool scythe_attack_flag_{ false };
	bool psyco1_attack_flag_{ false };
	bool psyco2_attack_flag_{ false };
	float attack_timer_{ 0.0f };
	float cool_timer_{ 0.0f };

	//�v���C���[�p�̓��ꕨ
	Actor* player_;
};

#endif