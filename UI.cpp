#include"UI.h"
#include"IWorld.h"
#include"Field.h"
#include"Assets.h"


//GUI‚Ì•`‰æ
void UI::draw_gui() const {
	Actor* player = world_->find_actor("Player");
	
	gsDrawSprite2D(Texture_Hp, NULL, NULL, NULL, NULL, NULL, 0);
}

