#include"GamePlayScene.h"
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
#include"MapGenerator.h"
#include "player_ghost.h"
#include"ParticleManager.h"



//�J�n
void GamePlayScene::start() {
    fade_.start(true,1);

    gsLoadShader(0, "Paladin.vert", "Paladin.frag");
    // �ς��������b�V���̓ǂݍ���
    gsLoadMeshEx(0, "Assets/Model/patti.msh");
	//����
    MapGenerator generator{ &world_,"Assets/Map/Stage1.csv"};

    is_end_ = false;
    // CarGhost�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_RushGhost, "Assets/Model/Enemy/Ghost2.msh");
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost3.msh");
    gsLoadSkeleton(Skeleton_CarGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_RushGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_Poltergeist, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_RushGhost, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_Poltergeist, "Assets/Model/Enemy/Ghost.anm");

    //SurogSakones�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakones�̃X�P���g���̓ǂݍ���
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakones�̃A�j���[�V�����̓ǂݍ���
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    //Player
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Skeleton_Player, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_Player, "Assets/Model/Enemy/Ghost.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");
    //�o���b�g(�{)�̃��b�V���̒ǉ�
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
	
    //�X�J�C�{�b�N�X�̓ǂݍ���
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/DarkStorm4K.msh");
    //�`��p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    //�Փ˔���p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");

    gsLoadSE(SE_Attack, "Assets/SE/punch.wav", 1, false);
    gsLoadSE(SE_Jump, "Assets/SE/Jump.wav",1,false);
    gsLoadSE(SE_GhostDamage, "Assets/SE/ghost_damage.wav", 1, false);
    gsLoadSE(SE_ParadinDamage, "Assets/SE/paradin_damage.wav", 1, false);
    //gsLoadSE(SE_GhostDamage, "Assets/SE/punch.wav", 1, false);
    //gsLoadSE(SE_ParadinDamage, "Assets/SE/Jump.wav", 1, false);
	
    gsLoadMusic(Music_GamePlay, "Assets/BGM/gameplay.wav", GS_TRUE);
    gsBindMusic(Music_GamePlay);
    gsPlayMusic();

    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // �J�����̒ǉ�
    world_.add_camera(new Camera{ &world_ });
    // ���C�g�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���̒ǉ�
    world_.add_actor(new player_ghost{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //�ǉ�
    world_.add_particle_manager(new ParticleManager{ &world_ });
}

//�X�V
void GamePlayScene::update(float delta_time) {
    gsSetMusicVolume(0.8f);
	world_.update(delta_time);

    if (world_.is_game_clear() && fade_.is_end()) {//�{�X�����񂾂��H
        is_end_ = true;
        next_scene_ = "ResultScene";
        fade_.change_fade_flg();
    }
    else if (world_.is_game_over() && fade_.is_end()) {//�v���C���[�����񂾂��H
        is_end_ = true;
        next_scene_ = "GameOverScene";
        fade_.change_fade_flg();
    }
    fade_.update(delta_time);
}

//�`��
void GamePlayScene::draw() const {
	world_.draw();
    fade_.draw();
}

//�I�����Ă��邩�H
bool GamePlayScene::is_end() const {
    if (fade_.is_end()) {
        return is_end_;
    }
    else return false;//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
	return next_scene_;
}
//�I��
void GamePlayScene::end() {
    gsStopMusic();
    gsDeleteMusic(Music_Title);
	//���[���h������
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