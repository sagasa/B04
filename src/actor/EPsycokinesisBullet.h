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

	/// <summary>
	/// Small_EPsycokinesis
	/// </summary>
	/// <param name="world"></param>
	/// <param name="position"></param>
	/// <param name="delay"></param>
	EPsycokinesisBullet(IWorld* world, Actor* player,const GSvector3& position,float delay=0.0f);
	/// <summary>
	/// Big_EPsycokinesis
	/// </summary>
	/// <param name="world">ワールド</param>
	/// <param name="position">位置</param>
	/// <param name="velocity">速度</param>
	EPsycokinesisBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity=GSvector3::zero(), float delay=0.0f);
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

	float life_timer_{ 0.0f };
	float generate_effect_{ 0.0f };
};

#endif