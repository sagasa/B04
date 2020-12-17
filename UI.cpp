#include"UI.h"
#include"IWorld.h"
#include"Field.h"
#include"Assets.h"
#include "Player.h"

UI::UI(IWorld* world)
	:hp_{ 3 },
	shield_hp_{ 0 }
{
	world_ = world;
}
void UI::update(float delta_time)
{
	player_ = world_->find_actor("Player");
	if (player_ == nullptr)return;
	if(gsGetKeyTrigger(GKEY_O))
	{
		hp_--;
	}
	if(gsGetKeyTrigger(GKEY_P))
	{
		hp_++;
	}
	if(gsGetKeyTrigger(GKEY_K))
	{
		shield_hp_--;
	}
	if(gsGetKeyTrigger(GKEY_L))
	{
		shield_hp_++;
	}
	//change_gui();
}


//GUI‚Ì•`‰æ
void UI::draw_gui() const {
	if (hp_ <= 0)return;
	static const GSvector2 heart_scale{ 0.1f,0.1f };
	static const GSvector2 shield_scale{ 0.3f,0.3f };
	static const GSvector2 heart_position{ 10.0f,50.0f };	
	static const GSvector2 shield_position{ 40.0f,20.0f };	
	static const float interval{ 160.0f };
	for(int i=0;i<(int)hp_;++i)
	{
		const GSvector2 position{ heart_position.x + interval * i,heart_position.y };
		gsDrawSprite2D(Texture_Hp, &position, NULL, NULL, NULL, &heart_scale, 0);
	}
	if (shield_hp_ <= 0)return;
	for (int i = 0; i < (int)shield_hp_; ++i)
	{
		const GSvector2 position{ shield_position.x + interval * i,shield_position.y };
		gsDrawSprite2D(Texture_Shield, &position, NULL, NULL, NULL, &shield_scale, 0);
	}
}

void UI::change_gui()
{
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;

	ActorProp& ap = dynamic_cast<ActorProp&>(*player);
	if (hp_ != (int)floorf(ap.hp()))
	{
		hp_ = (int)floorf(ap.hp());
	}
}

