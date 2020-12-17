#include"EnemyTestScene.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"NormalGhost.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Assets.h"
#include"SceneManager.h"
#include"SurogSakones.h"
#include<GSmusic.h>

#include "player_ghost.h"


//�J�n
void EnemyTestScene::start() {
    //BGM�̓ǂݍ���
    gsLoadMusic(Music_BackGround, "Assets/BGM/Horror-MusicBox.mp3",GS_TRUE);

    // Ghost�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadMesh(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_RushGhost, "Assets/Model/Enemy/Ghost2.msh");
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost3.msh");
    //�o���b�g(�{)�̃��b�V���̒ǉ�
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
    //Ghost�̃X�P���g���̓ǂݍ���
    gsLoadSkeleton(Skeleton_Player,"Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_CarGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_RushGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_Poltergeist, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    //Ghost�̃A�j���[�V�����̓ǂݍ���
    gsLoadAnimation(Animation_Player, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_RushGhost, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_Poltergeist, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");

    //SurogSakones�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakones�̃X�P���g���̓ǂݍ���
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakones�̃A�j���[�V�����̓ǂݍ���
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    //�X�J�C�{�b�N�X�̓ǂݍ���
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/DarkStorm4K.msh");
    //�`��p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    //�Փ˔���p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");
    

    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // �J�����̒ǉ�
    world_.add_camera(new Camera{ &world_ });
    // ���C�g�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���̒ǉ�
    world_.add_actor(new player_ghost{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });//x470
    //�G�l�~�[1
    world_.add_actor(new CarGhost{ &world_,GSvector3{10.0f,0.0f,0.0f} });
    //�G�l�~�[2
    world_.add_actor(new RushGhost{ &world_,GSvector3{5.0f,5.0f,0.0f} });
    //�G�l�~�[3
    world_.add_actor(new Poltergeist{ &world_,GSvector3{5.0f,0.0f,0.0f} });
    //�G�l�~�[4
    world_.add_actor(new NormalGhost{ &world_,GSvector3{15.0f,0.0f,0.0f} });
    //world_.add_actor(new SurogSakones{ &world_,GSvector3{15.0f,0.0f,0.0f} });

    //BGM�̃o�C���h
    gsBindMusic(Music_BackGround);
    gsSetMusicVolume(0.8f);
    //BGM�̍Đ�
    gsPlayMusic();
    
}

//�X�V
void EnemyTestScene::update(float delta_time) {
    world_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_RETURN)) {
    }
}

//�`��
void EnemyTestScene::draw() const {
    world_.draw();
}

//�I�����Ă��邩
bool EnemyTestScene::is_end() const {
	return is_end_;
}

//���̃V�[����Ԃ�
std::string EnemyTestScene::next() const {
    return "scene_null";

}

//�I��
void EnemyTestScene::end() {
    gsPauseMusic();
    gsDeleteMusic(Music_BackGround);
    world_.clear();
    //���b�V���̍폜
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_Paladin);
    gsDeleteMesh(Mesh_CarGhost);
    gsDeleteMesh(Mesh_Poltergeist);
    gsDeleteMesh(Mesh_RushGhost);
    gsDeleteMesh(Mesh_Book);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteMesh(Mesh_SurogSakones);
    //�X�P���g���̍폜
    gsDeleteSkeleton(Skeleton_CarGhost);
    gsDeleteSkeleton(Skeleton_Paladin);
    gsDeleteSkeleton(Skeleton_Poltergeist);
    gsDeleteSkeleton(Skeleton_RushGhost);
    gsDeleteSkeleton(Skeleton_SurogSakones);
    //�A�j���[�V�����̍폜
    gsDeleteAnimation(Animation_CarGhost);
    gsDeleteAnimation(Animation_Paladin);
    gsDeleteAnimation(Animation_Poltergeist);
    gsDeleteAnimation(Animation_RushGhost);
    gsDeleteAnimation(Animation_SurogSakones);
    //�I�N�c���[�̍폜
    gsDeleteOctree(Octree_Collider);
    gsDeleteOctree(Octree_Stage);

}