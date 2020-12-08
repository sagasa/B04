#include<gslib.h>
#include"PlayerTestScene.h"
#include"Assets.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Player.h"
#include"SurogSakones.h"

void PlayerTestScene::start() {
    is_end_ = false;
    // �v���[�����b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");

    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");

    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // �J�����̒ǉ�
    world_.add_camera(new Camera{ &world_ });
    // ���C�g�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���̒ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
}

//�X�V
void PlayerTestScene::update(float delta_time) {
    world_.update(delta_time);
}
//�`��
void PlayerTestScene::draw() const {
    world_.draw();
}
//�I�����Ă��邩�H
bool PlayerTestScene::is_end() const {
    return is_end_;
}
//���̃V�[������Ԃ�
std::string PlayerTestScene::next() const {
    return "NullScene";
}
//�I��
void PlayerTestScene::end() {
    world_.clear();
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_SurogSakones);
    gsDeleteSkeleton(Skeleton_SurogSakones);
    gsDeleteAnimation(Animation_SurogSakones);
	
    gsDeleteTexture(Texture_BgTileNebulaGreen);
    gsDeleteTexture(Texture_EffectFlash);
    gsDeleteTexture(Texture_EffectLazerCyan);
    gsDeleteTexture(Texture_EffectLazerOrange);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
}