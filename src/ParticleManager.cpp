#include "ParticleManager.h"
#include "Particle.h"
#include "Assets.h"
#include "IWorld.h"

ParticleManager::ParticleManager(IWorld* world)
{
	world_ = world;
	name_ = "ParticleManager";
	tag_ = "ParticleManagerTag";
}

void ParticleManager::spark(const GSvector3& position)const
{
	for (int i = 0; i < 30; ++i)
	{
		Particle* p = new Particle{ world_ };
		p->transform().position(position);
		p->velocity(GSvector3{ gsRandf(0.1f,0.3f),gsRandf(-0.05f,0.0f),gsRandf(-0.3f,0.3f) });
		p->lifespan_ = 30.0f;
		p->image_handle_ = Texture_EffectLazerOrange;
		p->start_scale_ = GSvector2{ 1.0f,0.1f };
		p->end_scale_ = GSvector2{ 0.1f,0.01f };
		p->color_ = GSvector3{ 1.0f,1.0f,1.0f };
		p->damp_ = 0.95f;
		p->angle_ = 90.0f;
		p->force_ = GSvector3{ 0.0f,-0.005f,0.0f };
		p->start_alpha_ = 1.0f;
		p->end_alpha_ = 0.0f;
		//ラムダ式の処理(お手本として残しておく)
		p->OnUpdate = [&, p](float delta_time) {};
		p->OnDeath = [&] {};
		world_->add_actor(p);
	}
}

void ParticleManager::smoke(const GSvector3& position)const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3{ gsRandf(-0.1f, 0.1f), gsRandf(-0.01f, -0.005f), 0.0f });
	p->lifespan_ = gsRandf(12.5f, 15.0f);
	p->image_handle_ = Texture_Smoke;
	GSvector2 scale{ 0.5f,0.5f };
	p->start_scale_ = GSvector2{ scale };
	p->end_scale_ = GSvector2{ scale };
	p->color_ = GSvector3{ 1.0f,1.0f,1.0f };
	p->damp_ = gsRandf(0.95f, 0.98f);
	p->angle_ = 90.0f;
	p->force_ = GSvector3{ 0.0f,0.0025f,0.0f };
	p->start_alpha_ = gsRandf(0.3f, 0.5f);
	p->end_alpha_ = 0.0f;
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	world_->add_actor(p);
}

void ParticleManager::distotion(const GSvector3& position)const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3::zero());
	p->lifespan_ = 30.0f;
	p->image_handle_ = Texture_Distotion;
	p->start_scale_ = GSvector2{ 0.0f,0.0f };
	p->end_scale_ = GSvector2{ 1.0f,1.0f };
	p->color_ = GSvector3{ 1.0f,1.0f,1.0f };
	p->damp_ = 0.95f;
	p->start_alpha_ = 1.0f;
	p->end_alpha_ = 0.0f;
	world_->add_actor(p);
}

void ParticleManager::dust(const GSvector3& position) const
{
	const int dustParticleNum = 10;
	for (int i = 0; i < dustParticleNum; ++i)
	{
		auto p = new Particle{ world_ };
		p->transform().position(position);
		p->velocity(GSvector3{ gsRandf(-0.05f, 0.05f), gsRandf(-0.01f, -0.005f), 0.0f });
		p->lifespan_ = gsRandf(12.5f, 15.0f);
		p->image_handle_ = Texture_Smoke;
		p->start_scale_ = GSvector2{ 0.4f,0.4f };
		p->end_scale_ = GSvector2{ 0.7f,0.7f };
		p->color_ = GSvector3{ 1.0f,1.0f,1.0f };
		p->damp_ = gsRandf(0.95f, 0.98f);
		p->angle_ = 90.0f;
		p->force_ = GSvector3{ 0.0f,0.0025f,0.0f };
		p->start_alpha_ = gsRandf(0.3f, 0.5f);
		p->end_alpha_ = 0.0f;
		p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
		p->sfactor = GL_SRC_ALPHA;
		p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
		world_->add_actor(p);
	}
}

void ParticleManager::hit(const GSvector3& position) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3::zero());
	p->lifespan_ = 10.0f;
	p->image_handle_ = Texture_Hit;
	p->start_scale_ = GSvector2{ 1.0f,1.5f };
	p->end_scale_ = GSvector2{ 1.0f,1.7f };
	p->angle_ = gsRandf(-45.0f, 45.0f);
	p->color_ = GSvector3{ 1.0f,1.0f,1.0f };
	world_->add_actor(p);
}

void ParticleManager::death_smoke(const GSvector3& position,const GSvector3& velocity) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3{ gsRandf(-0.025f,0.03f),gsRandf(-0.01f,-0.005f),0.0f });
	p->lifespan_ = 20.0f;
	p->image_handle_ = Texture_Smoke;
	p->start_scale_ = GSvector2{ 0.5f,0.5f };
	p->end_scale_ = GSvector2{ 1.2f,1.2f };
	p->angle_ = gsRandf(-45.0f, 45.0f);
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
	p->start_alpha_ = 0.5f;
	p->end_alpha_ = 0.0f;
	p->fade_in_time = 0.1f;
	p->fade_out_time = 0.5f;
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	p->damp_ = gsRandf(0.95f, 0.98f);
	p->force_ = GSvector3{ 0.0f,0.005f,0.0f };
	world_->add_actor(p);
}

void ParticleManager::death_smoke2(const GSvector3& position, const GSvector3& velocity) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3{ gsRandf(-0.025f,0.03f),gsRandf(-0.01f,-0.005f),0.0f });
	p->lifespan_ = 30.0f;
	p->image_handle_ = Texture_Smoke;
	p->start_scale_ = GSvector2{ 1.2f,1.2f };
	p->end_scale_ = GSvector2{ 4.0f,4.0f };
	p->angle_ = gsRandf(-90.0f, 90.0f);
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
	p->start_alpha_ = 0.5f;
	p->end_alpha_ = 0.0f;
	p->fade_in_time = 0.1f;
	p->fade_out_time = 0.5f;
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	p->damp_ = gsRandf(0.1f, 0.2f);
	p->force_ = GSvector3{ 0.0f,0.005f,0.0f };
	world_->add_actor(p);
}

void ParticleManager::boss_smoke(const GSvector3& position,const GSvector3& velocity) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3{ gsRandf(-0.025f,0.025f),gsRandf(-0.01f,-0.005f),0.0f });
	p->lifespan_ = gsRandf(20.0f, 30.0f);
	p->image_handle_ = Texture_Smoke;
	p->start_scale_ = GSvector2{ 0.3f,0.3f };
	p->end_scale_ = GSvector2{ 0.8f,0.8f };
	p->angle_ = gsRandf(-45.0f, 45.0f);
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
	p->start_alpha_ = 0.5f;
	p->end_alpha_ = 0.0f;
	p->fade_in_time = 0.1f;
	p->fade_out_time = 0.5f;
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	p->damp_ = gsRandf(0.95f, 0.98f);
	p->force_ = GSvector3{ 0.0f,0.004f,0.0f };
	world_->add_actor(p);
}

void ParticleManager::possession_light(const GSvector3& position) const
{
	auto p = new Particle{ world_ };

	//原点から球体の形
	p->transform().position(position + GSvector3{ gsRandf(-1.0f,1.0f),gsRandf(-0.8f,0.8f) ,gsRandf(-0.8f,0.8f) });
	GSvector3 direction = position - p->transform().position();
	p->velocity(direction.normalize() * gsRandf(0.02f, 0.05f));
	p->lifespan_ = gsRandf(30.0f, 45.0f);
	p->image_handle_ = Texture_Circle;
	float scale = gsRandf(0.6f, 1.0f);
	p->start_scale_ = GSvector2{ scale,scale };
	p->end_scale_ = GSvector2{ 1.0f,1.0f };
	p->start_alpha_ = 1.0f;
	p->end_alpha_ = 0.0f;
	
	//色は下のパラメーターをいじってください！！
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };

	//ラムダ式の処理(お手本として残しておく)
	p->OnUpdate = [&, p, position](float delta_time)
	{
		float dis = GSvector3::distance(p->transform().position(), position);
		//原点に近づいたら止める。
		if (GSvector3::distance(p->transform().position(), position + p->velocity()) <= 0.05f)
		{
			p->transform().position(position);
		}
	};
	p->OnDeath = [&] {};

	world_->add_actor(p);
}

void ParticleManager::possession_release_light(const GSvector3& position) const
{
	const int GenerateCount{ 25 };	
	for(int i=0;i<GenerateCount;++i)
	{
		auto p = new Particle{ world_ };
		p->transform().position(position);
		p->velocity(GSvector3{ gsRandf(-0.04f, 0.04f), gsRandf(0.05f, 0.1f), gsRandf(-0.04f, 0.04f) });
		p->lifespan_ = gsRandf(15.0f,30.0f);
		p->image_handle_ = Texture_Circle;
		float startscale = gsRandf(0.8f, 1.0f);
		float endscale = gsRandf(0.3f, 0.5f);
		p->start_scale_ = GSvector2{ startscale, startscale};
		p->end_scale_ = GSvector2{ endscale,endscale };
		p->damp_ = gsRandf(0.97f,0.99f);
		p->force_ = GSvector3{ 0.0f,-0.005f,0.0f };
		p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
		p->start_alpha_ = 1.0f;
		p->end_alpha_ = 0.0f;
		world_->add_actor(p);
	}	
}

void ParticleManager::psyco_bullet_small(const GSvector3& position, const GSvector3& velocity) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	GSvector3 vel = GSvector3{ gsRandf(-0.025f,0.025f),gsRandf(-0.01f,-0.005f),0.0f } - velocity;
	p->velocity(vel);
	if(velocity_==GSvector3::zero())p->lifespan_ = gsRandf(10.0f, 20.0f);
	else p->lifespan_ = gsRandf(5.0f, 115.0f);
	p->image_handle_ = Texture_Smoke;
	float start_scale_{ gsRandf(0.3f,0.5f) };
	float end_scale_{ gsRandf(0.6f,1.0f) };
	p->start_scale_ = GSvector2{ start_scale_,start_scale_ };
	p->end_scale_ = GSvector2{ end_scale_,end_scale_ };
	p->angle_ = gsRandf(-45.0f, 45.0f);
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
	p->start_alpha_ = 0.7f;
	p->end_alpha_ = 0.0f;
	p->fade_in_time = 0.1f;
	p->fade_out_time = 0.5f;
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	p->damp_ = gsRandf(0.95f, 0.98f);
	p->force_ = GSvector3{ 0.0f,0.002f,0.0f };
	world_->add_actor(p);
}

void ParticleManager::psyco_bullet_big(const GSvector3& position, const GSvector3& velocity) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	GSvector3 vel = GSvector3{ gsRandf(-0.025f,0.025f),gsRandf(-0.01f,-0.005f),0.0f } - velocity;
	p->velocity(vel);
	p->lifespan_ = 20.0f;
	p->image_handle_ = Texture_Smoke;
	p->start_scale_ = GSvector2{ 0.5f,0.5f };
	p->end_scale_ = GSvector2{ 1.2f,1.2f };
	p->angle_ = gsRandf(-45.0f, 45.0f);
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
	p->start_alpha_ = 0.5f;
	p->end_alpha_ = 0.0f;
	p->fade_in_time = 0.1f;
	p->fade_out_time = 0.5f;
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	p->damp_ = gsRandf(0.95f, 0.98f);
	if(velocity==GSvector3::zero())p->force_ = GSvector3{ 0.0f,0.005f,0.0f };
	world_->add_actor(p);
}

void ParticleManager::boss_retreat(const GSvector3& position) const
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3{ gsRandf(-0.04f,0.04f),gsRandf(-0.01f,-0.005f),0.0f });
	p->lifespan_ = gsRandf(20.0f,35.0f);
	p->image_handle_ = Texture_Smoke;
	float startscale = gsRandf(2.0f, 3.0f);
	p->start_scale_ = GSvector2{ startscale,startscale };
	float endscale = gsRandf(3.5f, 4.0f);
	p->end_scale_ = GSvector2{ endscale,endscale };
	p->angle_ = gsRandf(-45.0f, 45.0f);
	p->angular_velocity_ = gsRandf(-5.0f, 5.0f);
	p->color_ = GSvector3{ gsRandf(0.3f,0.5f),0.0f,gsRandf(0.3f,0.5f) };
	p->start_alpha_ = 0.5f;
	p->end_alpha_ = 0.0f;
	p->fade_in_time = 0.1f;
	p->fade_out_time = 0.5f;
	p->sfactor = GL_SRC_ALPHA;
	p->dfactor = GL_ONE_MINUS_SRC_ALPHA;
	p->damp_ = gsRandf(0.95f, 0.98f);
	p->force_ = GSvector3{ 0.0f,0.02f,0.0f };
	world_->add_actor(p);
}


