#include "player_ghost.h"

player_ghost::player_ghost():player_info(AnimatedMesh{ Mesh_Poltergeist, Skeleton_CarGhost, Animation_CarGhost })
{

}

bool player_ghost::attack()
{
	return false;
}

void player_ghost::draw()
{
    mesh_.draw();
}

bool player_ghost::hit()
{
	return false;
}

void player_ghost::update(Actor& player, float delta)
{
    //モーション変更
    mesh_.change_motion(motion_);
    //メッシュの更新
    mesh_.update(delta);
    //行列を設定
    mesh_.transform(player.transform().localToWorldMatrix());
}
