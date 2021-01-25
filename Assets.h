#pragma once


// ���b�V���f�[�^
enum Mesh {
    Mesh_Player,                    // ���@
    Mesh_CarGhost,                  //CarGhost
    Mesh_RushGhost,                 //RushGhost
    Mesh_Poltergeist,               //Poltergeist
    Mesh_SurogSakones,               //SurogSakones
    Mesh_Skybox,
    Mesh_Paladin,
    Mesh_Book,
    Mesh_Ghost
};

//�X�P���g���f�[�^
enum Skeleton {
    Skeleton_Player,       //�v���C���[
    Skeleton_CarGhost,     //CarGhost
    Skeleton_RushGhost,    //RushGhost
    Skeleton_Poltergeist,  //Poltergeist
    Skeleton_SurogSakones, //SurogSakones
    Skeleton_Paladin
};

//�A�j���[�V�����f�[�^
enum  Animation {
    Animation_Player,      //�v���C���[
    Animation_CarGhost,    //CarGhost
    Animation_RushGhost,   //RushGhost
    Animation_Poltergeist, //Poltergeist
    Animation_SurogSakones, //SurogSakones
    Animation_Paladin
};

//�V�F�[�_�[�f�[�^
enum Shader {
    Shader_Ghost,
    Shader_Paladin,
    Shader_Map
};

// �e�N�X�`���f�[�^
enum Texture {
    //UI
    Texture_Hp,                     //�̗�(�n�[�g)
    Texture_Shield,                 //�̗�(��)
    //����
    Texture_TitleLogo,              //�^�C�g�����S
    Texture_PushButton,            //�{�^��
    Texture_GameClear,                  //�Q�[���N���A
    Texture_GameOver,              //�Q�[���I�[�o�[
    Texture_TitleButton,        //�^�C�g���֖߂�
    Texture_RestartButton,               //������x
    Texture_NextStage,             //���̃X�e�[�W��
    //�t�F�[�h
    Texture_Fade,                   //�����e�N�X�`��
    // 3D�X�v���C�g
    Texture_EffectLazerCyan,        // ���[�U�[�i���F�j
    Texture_EffectLazerOrange,      // ���[�U�[�i�I�����W�F�j
    Texture_EffectFlash,             // �����t���b�V��
    Texture_Smoke,
	Texture_Distotion,
	Texture_Hit,
	Texture_Circle,
	Texture_BossHP
};

// �a�f�l�f�[�^
enum Music {
    Music_Title,                    //�^�C�g��BGM
    Music_GamePlay,                 //�Q�[���v���CBGM
    Music_GameOver,                 //�Q�[���I�[�o�[BGM
    Music_GameClear                 //�Q�[���N���ABGM
};

// �r�d�f�[�^
enum Se {
    SE_GhostDamage,                 //�H��̃_���[�WSE
    SE_GhostMove,                   //�H��̈ړ�SE
    SE_ParadinMove,                 //�߈˂̈ړ�SE
    SE_ParadinDamage,               //�߈˂̃_���[�WSE
    SE_Attack,                      //�U��SE
    SE_Jump,                        //�W�����vSE
    SE_Transfer,                    //�߈�SE
    SE_Slash,                       //�a��SE
    SE_Shoot,                       //����SE
    SE_Select,                      //�I���ړ�SE
    SE_Push                         //����SE

};

enum Octree {
    Octree_Stage,
    Octree_Collider
};
