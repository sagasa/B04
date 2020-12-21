#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <functional>

#include "Actor.h"

class Particle :
	public Actor
{
public:
	enum class State
	{
		//待機
		Sleep,
		//通常
		Active,
	};
public:
	Particle(IWorld* world);
	virtual void update(float delta_time)override;
	virtual void draw_transparent() const override;

	std::function<void()> OnUpdate;
	std::function<void()> OnDeath;
	
public:
	bool is_dead_{ false };
	float lifespan_{ 0.0f };
	float age_{ 0.0f };
	GSuint image_handle_{ 0 };
	GSvector3 force_{ GSvector3::zero() };
	
	//スケール
	GSvector2 start_scale_{ GSvector2::one() };
	GSvector2 end_scale_{ GSvector2::one() };
	
	//カラー
	GSvector3 start_color_{ 1.0f,1.0f,1.0f};
	GSvector3 end_color_{ 1.0f,1.0f,1.0f };

	//フェードイン、フェードアウトのタイマー
	float fade_in_time{ 0.0f };
	float fade_out_time{ 0.0f };

	//角度	
	float angle_{ 0.0f };

	//空気抵抗
	float damp_{ 1.0f };

	//回転
	float angular_velocity_{ 0.0f };
	float angular_damp_{ 1.0f };

	//ブレンドモード
	int sfactor{ GL_SRC_ALPHA };
	int dfactor{ GL_ONE };

	//遅延時間
	float delay_timer_{ 0.0f };

private:
	State state_{ State::Sleep };
	GSvector2 scale_{ GSvector2::one() };
	GSvector3 color_{ 1.0f,1.0f,1.0f };
	float alpha_{ 1.0f };
};

#endif