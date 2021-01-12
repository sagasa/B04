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

void ParticleManager::spark(const GSvector3& position)
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
		//ƒ‰ƒ€ƒ_Ž®‚Ìˆ—(‚¨Žè–{‚Æ‚µ‚ÄŽc‚µ‚Ä‚¨‚­)
		p->OnUpdate = [&, p](float delta_time) {};
		p->OnDeath = [&] {};
		world_->add_actor(p);
	}
}

void ParticleManager::smoke(const GSvector3& position)
{
	auto p = new Particle{ world_ };
	p->transform().position(position);
	p->velocity(GSvector3{ gsRandf(-0.3f, 0.3f), gsRandf(-0.3f, 0.3f), gsRandf(-0.3f, 0.3f) });
	p->lifespan_ = 30.0f;
	p->image_handle_ = Texture_Smoke;
	p->start_scale_ = GSvector2{ 1.0f,1.0f };
	p->end_scale_ = GSvector2{ 1.0f,1.0f };
	p->color_ = GSvector3{ 1.0f,1.0f,1.0f };
	p->damp_ = 0.95f;
	p->angle_ = 90.0f;
	p->force_ = GSvector3{ 0.0f,0.005f,0.0f };
	p->start_alpha_ = 1.0f;
	p->end_alpha_ = 0.0f;
	p->angular_velocity_ = 5.0f;
	world_->add_actor(p);
}

void ParticleManager::distotion(const GSvector3& position)
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

