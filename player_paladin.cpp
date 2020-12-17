#include "player_paladin.h"


player_paladin::player_paladin(IWorld* world, const GSvector3& position) :Player(world, position, AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
	
}

void player_paladin::update(float delta)
{
    //モーション変更
    mesh_.change_motion(motion_);
    //メッシュの更新
    mesh_.update(delta);
    //行列を設定
    mesh_.transform(transform().localToWorldMatrix());
}
