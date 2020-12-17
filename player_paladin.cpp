#include "player_paladin.h"


player_paladin::player_paladin(IWorld* world, const GSvector3& position) :Player(world, position, AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
	
}

void player_paladin::on_hit(const Actor& attacker, float atk_power)
{
}

void player_paladin::update(float delta)
{
    //���[�V�����ύX
    mesh_.change_motion(motion_);
    //���b�V���̍X�V
    mesh_.update(delta);
    //�s���ݒ�
    mesh_.transform(transform().localToWorldMatrix());
}
