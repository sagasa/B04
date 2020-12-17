#include<gslib.h>
#include"BossTestScene.h"
#include"Assets.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Player.h"
#include"SurogSakones.h"
#include "UI.h"
#include "AttackCollider.h"

void BossTestScene::start() {
    // �v���[���[�e�摜��ǂݍ���
    gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
    // �G�e�摜��ǂݍ���
    gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
    gsLoadTexture(Texture_Hp, "Assets/Image/outline_heart.png");
    gsLoadTexture(Texture_Shield, "Assets/Image/shield.png");
    // �v���[�����b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");
    
    //SurogSakones�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakones�̃X�P���g���̓ǂݍ���
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakones�̃A�j���[�V�����̓ǂݍ���
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");

    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");

    //�o���b�g(�{)�̃��b�V���̒ǉ�
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
	
    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // �J�����̒ǉ�
    world_.add_camera(new Camera{ &world_ });
    // ���C�g�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���̒ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //�{�X
    world_.add_actor(new SurogSakones{ &world_,GSvector3{5.0f,0.0f,0.0f} });
    world_.add_actor(new UI{ &world_ });
}

//�X�V
void BossTestScene::update(float delta_time) {
	if(gsGetKeyTrigger(GKEY_RETURN))
	{
        BoundingSphere collider{ 0.5f,GSvector3{5.0f,-1.0f,0.0f} };
        world_.add_actor(new AttackCollider{ &world_,collider,"PlayerAttack","PlayerAttackName",60.0f,0.0f,10.0f });
	}
    world_.update(delta_time);
}
//�`��
void BossTestScene::draw() const {
    world_.draw();
}
//�I�����Ă��邩�H
bool BossTestScene::is_end() const {
	return false;
}
//���̃V�[������Ԃ�
std::string BossTestScene::next() const {
	return "NullScene";
}
//�I��
void BossTestScene::end() {
    world_.clear();
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_SurogSakones);
    gsDeleteSkeleton(Skeleton_SurogSakones);
    gsDeleteAnimation(Animation_SurogSakones);
    gsDeleteTexture(Texture_EffectFlash);
    gsDeleteTexture(Texture_EffectLazerCyan);
    gsDeleteTexture(Texture_EffectLazerOrange);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
}