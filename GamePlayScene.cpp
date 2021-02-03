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
#include"player_paladin.h"
#include<string>
#include<sstream>



//�J�n
void GamePlayScene::start(int number) {
    fade_.start(true,1);
    stage_number_ = number;

    gsLoadShader(0, "Paladin.vert", "Paladin.frag");
    // �ς��������b�V���̓ǂݍ���
    //gsLoadMeshEx(0, "Assets/Model/patti.msh");
    //�I���t���O������
    is_end_ = false;
    //�\���^�C�}�[�̏�����
    draw_timer_ = 0.0f;
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

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin_j_nordstrom/paladin_j_nordstrom.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin_j_nordstrom/paladin_j_nordstrom.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin_j_nordstrom/paladin_j_nordstrom.anm");
    //�o���b�g(�{)�̃��b�V���̒ǉ�
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
	
    //�X�J�C�{�b�N�X�̓ǂݍ���
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/DarkStorm4K.msh");
    //int�^�̃X�e�[�W�ԍ���stringstream�^�ɕϊ�
    std::stringstream ss;
    ss << stage_number_;
    //�X�e�[�W�ԍ��ɂ���ă��[�h����CSV�t�@�C����ύX����
    std::string csv_pass;
    csv_pass = "Assets/Map/Stage" + ss.str() + ".csv";
    //����
    MapGenerator generator{ &world_,csv_pass};
    //MapGenerator generator{ &world_,"Assets/Map/Stage3.csv" };
    //�ԍ��ɂ���ă��[�h����}�b�v��ς���
    std::string octree_pass;
    octree_pass = "Assets/Octree/stage"+ ss.str() +"/stage"+ ss.str()+".oct";
    std::string collide_pass = "Assets/Octree/stage" + ss.str() + "/stage" + ss.str() + "_collide.oct";
    //�`��p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Stage, octree_pass.c_str());
    //�Փ˔���p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Collider, collide_pass.c_str());
    //�`��p�I�N�c���[�̓ǂݍ���
    /*gsLoadOctree(Octree_Stage, "Assets/Octree/stage3/stage3.oct");
    //�Փ˔���p�I�N�c���[�̓ǂݍ���
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage3/stage3.oct");*/

	//�p�[�e�B�N���p�̃e�N�X�`��
    gsLoadTexture(Texture_Smoke, "Assets/Effect/particle_smoke.png");
    gsLoadTexture(Texture_Hit, "Assets/Effect/particle_glitter_3.png");
    gsLoadTexture(Texture_Circle, "Assets/Effect/particle_dot_1.png");
    gsLoadTexture(Texture_BossHP, "Assets/Image/BossHP.png");
    gsLoadTexture(Texture_BossHPFrame, "Assets/Image/BossHPFrame.png");
    gsLoadTexture(Texture_BossHPFrame_Lid, "Assets/Image/BossHPFrame_Lid.png");

	//SE
    gsLoadSE(SE_Attack, "Assets/SE/punch.wav", 1, false);
    gsLoadSE(SE_Jump, "Assets/SE/Jump.wav",1,false);
    gsLoadSE(SE_GhostDamage, "Assets/SE/ghost_damage.wav", 1, false);
    gsLoadSE(SE_ParadinDamage, "Assets/SE/paradin_damage.wav", 1, false);
    //gsLoadSE(SE_GhostDamage, "Assets/SE/punch.wav", 1, false);
    //gsLoadSE(SE_ParadinDamage, "Assets/SE/Jump.wav", 1, false);
    gsLoadSE(SE_P_Ghost_Attack, "Assets/SE/p_ghost_Attack.wav", 1, false);

    gsLoadTexture(Texture_Hp, "Assets/Image/heart.dds");
    gsLoadTexture(Texture_Shield, "Assets/Image/shield.dds");
    
    gsLoadSE(SE_GhostAttack1, "Assets/SE/Ghost Attack_01.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_GhostAttack2, "Assets/SE/Ghost Attack_02.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_BossGhostDamage, "Assets/SE/Ghost Damaged_01.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_GhostDeath, "Assets/SE/Ghost Death_02.wav", 1, GWAVE_DEFAULT);

    gsLoadSE(SE_Slash, "Assets/SE/sword-drawn1.wav", 1, GWAVE_DEFAULT);

    gsLoadSE(SE_Shoot, "Assets/SE/shoot.wav", 1, GWAVE_DEFAULT);
	
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
    //���ʒ���
    gsSetMusicVolume(0.8f);
    //���[���h�̍X�V
	world_.update(delta_time);
    //�t�F�[�h�N���X�̍X�V
    fade_.update(delta_time);

    if (fade_.is_end()) {
        //�\���^�C�}�[�̍X�V
        draw_timer_ += delta_time;
    }

    if (!is_end_ && world_.is_game_clear() && fade_.is_end()) {//�{�X�����񂾂��H
        is_end_ = true;
        next_scene_ = "ResultScene";
        fade_.change_fade_flg();
        next_scene_ = (stage_number_ == 3) ? "ResultScene" : "StageClearScene";
    }
    else if (!is_end_ && world_.is_game_over() && fade_.is_end()) {//�v���C���[�����񂾂��H
        is_end_ = true;
        next_scene_ = "GameOverScene";
        fade_.change_fade_flg();
    }
    
}

//�`��
void GamePlayScene::draw() const {
    //���[���h�̕`��
	world_.draw();
    //�X�e�[�W����\��

    //�t�F�[�h�N���X�̕`��
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

//���݂̃X�e�[�W�ԍ���Ԃ�
int GamePlayScene::stage_number() const {
    return stage_number_;
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

    gsDeleteSE(SE_Attack);
    gsDeleteSE(SE_Jump);
    gsDeleteSE(SE_ParadinDamage);
    gsDeleteSE(SE_GhostDeath);
    gsDeleteSE(SE_GhostDamage);
    gsDeleteSE(SE_GhostAttack1);	
    gsDeleteSE(SE_GhostAttack2);	
    gsDeleteSE(SE_BossGhostDamage);
    gsDeleteSE(SE_Shoot);
    gsDeleteSE(SE_Slash);
    gsDeleteSE(SE_P_Ghost_Attack);
    gsDeleteTexture(Texture_BossHPFrame);
    gsDeleteTexture(Texture_BossHP);
    gsDeleteTexture(Texture_BossHPFrame_Lid);
    gsDeleteTexture(Texture_Circle);
    gsDeleteTexture(Texture_Smoke);
    gsDeleteTexture(Texture_Hit);
}