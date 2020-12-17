#include "player_paladin.h"


void player_paladin::wake_up()
{
}

void player_paladin::stop()
{
}

player_paladin::player_paladin(IWorld* world, const GSvector3& position) :Player(world, position, AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
	
}

bool player_paladin::on_hit(const Actor& attacker, float atk_power)
{
    return false;
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
