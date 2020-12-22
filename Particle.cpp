#include "Particle.h"

Particle::Particle(IWorld* world)
{
	world_ = world;
}

void Particle::update(float delta_time)
{
	if (delay_timer_ > 0.0f)
	{
		delay_timer_ -= delta_time;
		return;
	}
	state_ = State::Active;
	//�o�ߎ���(�t���[��)
	age_ += delta_time;

	if (age_ > lifespan_)
	{
		if (OnDeath)OnDeath();
		die();
		return;
	}

	//�o�ߎ��Ԃɂ��B����
	const float progressRate = age_ / lifespan_;

	//���`���
	if(start_scale_!=end_scale_)
	{
		scale_ = GSvector2::lerp(start_scale_, end_scale_, progressRate);
	}	
	if(start_color_!=end_color_)
	{
		color_ = GSvector3::lerp(start_color_, end_color_, progressRate);
	}
	
	alpha_ = MIN(MIN(progressRate / fade_in_time, (1.0f - progressRate) / fade_out_time), 1.0f) * alpha_;

	//��]�֌W
	angular_velocity_ *= angular_damp_;
	angle_ += angular_velocity_;

	//�ړ��֌W
	velocity_ += force_;
	damp_ = CLAMP(damp_, 0.0f, 1.0f);
	velocity_ *= damp_;
	transform_.translate(velocity_ * delta_time);

	if (OnUpdate)OnUpdate();
}

void Particle::draw_transparent() const
{
	//����ł��炵�Ȃ�
	if (state_ != State::Active)return;

	//�摜�̑傫��
	static const GSrect body{ -0.5f, 0.5f, 0.5f, -0.5f };

	//�\��
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glBlendFunc(sfactor, dfactor);
	gsDrawSprite3D(
		image_handle_,
		&transform_.position(),
		&body,
		NULL,
		&GScolor{ color_,alpha_ },
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

