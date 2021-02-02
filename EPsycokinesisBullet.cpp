#include "EPsycokinesisBullet.h"

#include "Assets.h"
#include "ParticleManager.h"
#include "DamageProp.h"
#include "IWorld.h"
#include "Field.h"

const GSvector3 offset{ 0.0f,0.5f,0.0f };
const GSvector3 PlayerOffset{ 0.0f,1.0f,0.0f };

//¬‚³‚¢‹…ŠÖŒW
const float SmallAppearTime{ 60.0f };
const float SmallSpeed{ 0.075f };

//‘å‚«‚¢‹…ŠÖŒW
const float MaxAcceleration{ 3.0f };
const float Period{ 3.0f };

EPsycokinesisBullet::EPsycokinesisBullet(IWorld* world,Actor* player, const GSvector3& position, float delay)
	:delay_{delay},type_{ Type::Small },life_timer_{7.0f}
{
	name_ = "EPsycokinesisBullet";
	tag_ = "EnemyAttack";
	world_ = world;
	transform_.position(position);
	velocity_ = GSvector3::zero();
	collider_ = BoundingSphere{ 0.1f };
	to_player_ = (player->transform().position() + PlayerOffset) - (transform_.position() + offset);
}
EPsycokinesisBullet::EPsycokinesisBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity,float delay)
	:type_{ Type::Big }, life_timer_{ 7.0f }, period_{Period}, delay_{delay}
{
	name_ = "EPsycokinesisBullet";
	tag_ = "EnemyAttack";
	world_ = world;
	transform_.position(position);
	velocity_ = velocity;
	life_timer_ = 7.0f;
	collider_ = BoundingSphere{ 0.25f };
}

void EPsycokinesisBullet::update(float delta_time)
{
	player_ = world_->find_actor("Player");
	if (player_ == nullptr)
	{
		player_ = world_->find_actor("PlayerPaladin");
		if (player_ == nullptr)return;
	}
	
	delay_ -= delta_time;	
	if (delay_ >= 0.0f) {
		to_player_ = (player_->transform().position() + PlayerOffset) - (transform_.position() + offset);
		return;
	}
	
	life_timer_ -= delta_time / 60.0f;
	if (life_timer_ <= 0.0f)die();	
	
	if (world_->field()->collide(collider()))
	{
		die();
	}

	switch (type_)
	{
	case Type::Small:small_update(delta_time); break;
	case Type::Big:big_update(delta_time); break;
	}
	wait_timer_ += delta_time;
}

void EPsycokinesisBullet::draw() const
{
#ifdef _DEBUG
	collider().draw();
#endif
	switch (type_)
	{
	case Type::Small:
		world_->particle_manager()->psyco_bullet_small(transform_.position(), velocity_);
		world_->particle_manager()->psyco_bullet_small(transform_.position(), velocity_);
		break;
	case Type::Big:big_draw(); break;
	}
}

void EPsycokinesisBullet::react(Actor& other)
{

	if (other.tag() == "PlayerTag")
	{
		if (DamageProp::do_attack(other, *this, 1))die();
	}
}

void EPsycokinesisBullet::small_update(float delta_time)
{
	to_player_.normalize();
	velocity_ = to_player_ * SmallSpeed * delta_time;
	transform_.translate(velocity_);
}

void EPsycokinesisBullet::big_draw() const
{
	world_->particle_manager()->death_smoke(transform_.position());
}

void EPsycokinesisBullet::big_update(float delta_time)
{
	acceleration_ = GSvector3::zero();
	GSvector3 diff = player_->transform().position() + PlayerOffset - transform_.position();
	acceleration_ += ((diff - velocity_ * period_) * 2.0f / (period_ * period_));
	if (acceleration_.magnitude() >= MaxAcceleration) {
		acceleration_ = acceleration_.normalize() * MaxAcceleration;
	}
	period_ -= (delta_time / 60.0f);
	velocity_ += acceleration_ * (delta_time / 60.0f);

	transform_.position(transform_.position() + velocity_ * (delta_time / 60.0f));
}

