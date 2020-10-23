#ifndef SUROG_SAKONES
#define SUROG_SAKONES

class IWorld;

#include "Actor.h"
#include<vector>

class SurogSakones :
	public Actor
{
public:
	enum class State {
		Unkown,
		Apper,
		Normal,
		Angry,
		Stun,
		Die,
	};
public:
	SurogSakones(IWorld* world, const GSvector3& position);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	void Damage();

private:
	void appear_update(float delta_time);
	void normal_update(float delta_time);
	void angry_update(float delta_time);
	void stun_update(float delta_time);
	void die_update(float delta_time);
	//”O“®UŒ‚
	void pshychokinesis(const GSvector3& position);
	//•’Ê‚ÌˆÚ“®(”½‘Î‘¤‚ÉˆÚ“®‚·‚é)
	void move(float delta_time);
	//ˆÚ“®‚µ‚Â‚ÂUŒ‚
	void move_attack(float delta_time);
	//ˆÚ“®‚µ‚½Œã‚Ìƒ^[ƒ“
	void turn(float delta_time,const GSvector3& toRotate,float slow_value);
	void debug_draw()const;	

private:
	State state_ = State::Unkown;
	float hp_{ 0.0f };
	std::vector<GSvector3> move_pos_;
	GSvector3 destination_;

};

#endif