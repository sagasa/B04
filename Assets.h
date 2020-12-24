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
    Mesh_Book
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

// �e�N�X�`���f�[�^
enum Texture {
    //UI
    Texture_Hp,                     //�̗�(�n�[�g)
    Texture_Shield,                 //�̗�(��)
    Texture_TitleLogo,              //�^�C�g�����S
    Texture_push_button,            //�{�^��,
    Texture_Fade,                   //�����e�N�X�`��
    // 3D�X�v���C�g
    Texture_EffectLazerCyan,        // ���[�U�[�i���F�j
    Texture_EffectLazerOrange,      // ���[�U�[�i�I�����W�F�j
    Texture_EffectFlash             // �����t���b�V��
};

// �a�f�l�f�[�^
enum Music {
    Music_BackGround                // BGM
};

// �r�d�f�[�^
enum Se {
    Se_ExplosionPlayer,             // �������i���@�j
    Se_ExplosionEnemy,              // �������i�G�j
    Se_ExplosionAsteroid,           // �������i覐΁j
    Se_WeaponPlayer,                // ���ˉ��i���@�j
    Se_WeaponEnemy,                 // �������i�G�j
};

enum Octree {
    Octree_Stage,
    Octree_Collider
};
