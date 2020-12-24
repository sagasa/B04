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
		p->transform().position(GSvector3::zero());
		p->velocity(GSvector3{ gsRandf(0.05f,0.2f),gsRandf(-0.03f,0.0f),0.0f });
		p->lifespan_ = 30.0f;
		p->image_handle_ = Texture_EffectLazerCyan;
		p->start_scale_ = GSvector2{ 0.4f,0.4f };
		p->end_scale_ = GSvector2{ 0.1f,0.1f };
		p->color_ = GSvector3{ 1.0f,1.0f,0.0f };
		p->damp_ = 0.95f;
		p->angle_ = 90.0f;
		p->force_ = GSvector3{ 0.0f,-0.005f,0.0f };
		world_->add_actor(p);
	}
}