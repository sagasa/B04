#include "player_paladin.h"


player_paladin::player_paladin() :player_info(AnimatedMesh{ Mesh_Poltergeist, Skeleton_CarGhost, Animation_CarGhost })
{

}

bool player_paladin::attack()
{
	return false;
}

void player_paladin::draw()
{

}

bool player_paladin::hit()
{
	return false;
}

void player_paladin::update(Actor& player, float delta)
{

}
