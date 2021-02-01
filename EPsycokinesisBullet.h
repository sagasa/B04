#ifndef E_PSYCOKINESIS_BULLET_H_
#define E_PSYCOKINESIS_BULLET_H_
#include <functional>

#include "Actor.h"

class EPsycokinesisBullet :
    public Actor
{
public:
	enum class Type
	{
		Small,
		Big
	};
	enum class State
	{
		Appear,
		Normal
	};
	
	EPsycokinesisBullet(IWorld* world,const GSvector3& position,Type type,float delay=0.0f);
	void react(Actor& other) override;
	void update(float delta_time) override;
	void draw() const override;

private:
	void small_update(float delta_time);
	void big_update(float delta_time);
	void small_draw()const;
	void big_draw()const;
	void big_intialize();

private:
	Type type_;
	float wait_timer_{ 0.0f };
	Actor* player_;
	GSvector3 to_player_{ GSvector3::zero() };
	float delay_{0.0f};
	
	GSvector3 acceleration_{ GSvector3::zero() };
	float period_{ 0.0f };
	State state_{ State::Appear };
};

#endif