#include "Particle.h"

Particle::Particle(IWorld* world)
{
	world_ = world;
	enable_collider_ = false;
}

void Particle::update(float delta_time)
{
	if (delay_timer_ > 0.0f)
	{
		delay_timer_ -= delta_time;
		return;
	}
	state_ = State::Active;
	//経過時間(フレーム)
	age_ += delta_time;

	if (age_ > lifespan_)
	{
		if (OnDeath)OnDeath();
		die();
		return;
	}

	//経過時間による達成率
	const float progressRate = age_ / lifespan_;

	scale_ = start_scale_;	
	//線形補間
	if(start_scale_!=end_scale_)
	{
		scale_ = GSvector2::lerp(start_scale_, end_scale_, progressRate);
	}	
	if(start_color_!=end_color_)
	{
		color_ = GSvector3::lerp(start_color_, end_color_, progressRate);
	}
	if(fabsf(start_alpha_-end_alpha_)>FLT_EPSILON)
	{
		alpha_ = LERP(progressRate, start_alpha_, end_alpha_);
	}

	alpha_ = MIN(MIN(progressRate / fade_in_time, (1.0f - progressRate) / fade_out_time), 1.0f) * alpha_;

	//回転関係
	angular_velocity_ *= angular_damp_;
	angle_ += angular_velocity_;

	//移動関係
	velocity_ += force_;
	damp_ = CLAMP(damp_, 0.0f, 1.0f);
	velocity_ *= damp_;
	transform_.translate(velocity_ * delta_time);

	if (OnUpdate)OnUpdate(delta_time);
}

void Particle::draw_transparent() const
{
	//死んでたらしない
	if (state_ != State::Active)return;

	//画像の大きさ
	static const GSrect body{ -0.5f, 0.5f, 0.5f, -0.5f };

	//表示
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glBlendFunc(sfactor, dfactor);
	GSvector3 position = transform_.position();
	GScolor color = GScolor{ color_,alpha_ };
	gsDrawSprite3D(
		image_handle_,
		&position,
		&body,
		NULL,
		&color,
		&scale_,
		angle_
	);
	glPopAttrib();
}

//void Particle::transform(const GSmatrix4& matrix)
//{
//	glPushMatrix();
//	glMultMatrixf(matrix);	
//	glPopMatrix();
//}

