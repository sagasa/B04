#include "PaladinSpawn.h"
#include "player_paladin.h"
#include "IWorld.h"

const float GenerateTime{ 3.0f };

PaladinSpawn::PaladinSpawn(IWorld* world, const GSvector3& position)
	:generate_timer_{0.0f}
{
	name_ = "PaladinSpawn";
	tag_ = "PaladinSpawnTag";
	world_ = world;	
	transform_.position(position);
	enable_collider_ = false;
	generate_paladin();
}

void PaladinSpawn::update(float delta_time)
{
	if(notice_)
	{
		generate_timer_ -= delta_time / 60.0f;
		if(generate_timer_<=0.0f&&notice_)
		{
			notice_ = false;
			generate_paladin();
		}
	}
}


void PaladinSpawn::generate_paladin()
{
	world_->add_actor(new player_paladin{ world_,transform_.position(),this });
}

void PaladinSpawn::notice()
{
	notice_ = true;
	generate_timer_ = GenerateTime;
}