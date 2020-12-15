#include "player_paladin.h"


player_paladin::player_paladin() :player_info(AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
	move_speed = 0.8;
	attack_interval = 2;
}

bool player_paladin::attack()
{
	return false;
}

void player_paladin::draw()
{
    mesh_.draw();
}

bool player_paladin::hit()
{
	return false;
}

void player_paladin::update(const Actor& player, float delta)
{
    //モーション変更
    mesh_.change_motion(motion_);
    //メッシュの更新
    mesh_.update(delta);
    //行列を設定
    mesh_.transform(player.transform().localToWorldMatrix());
}
