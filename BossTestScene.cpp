#include<gslib.h>
#include"BossTestScene.h"
#include"Assets.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Player.h"
#include"SurogSakones.h"

void BossTestScene::start() {
    // �w�i�p�摜�̓ǂݍ���
    gsLoadTexture(Texture_BgTileNebulaGreen, "Assets/BG/tile_nebula_green_dff.png");
    // �v���[���[�e�摜��ǂݍ���
    gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
    // �G�e�摜��ǂݍ���
    gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
    // �v���[�����b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Player, "Assets/Model/vehicle_playerShip.msh");
    // CarGhost�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_CarGhost, "Assets/Model/Ghost.msh");
    // RushGhost�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_RushGhost, "Assets/Model/Ghost.msh");
    //Poltergeist�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Ghost.msh");
    //SurogSakones�̃��b�V���̓ǂݍ���
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakones�̃X�P���g���̓ǂݍ���
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakones�̃A�j���[�V�����̓ǂݍ���
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Texture_BgTileNebulaGreen });
    // �J�����̒ǉ�
    world_.add_camera(new Camera{ &world_ });
    // ���C�g�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���̒ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //�{�X
    world_.add_actor(new SurogSakones{ &world_,GSvector3{25.0f,-30.0f,0.0f} });
}

//�X�V
void BossTestScene::update(float delta_time) {
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
    gsDeleteTexture(Texture_BgTileNebulaGreen);
    gsDeleteTexture(Texture_EffectFlash);
    gsDeleteTexture(Texture_EffectLazerCyan);
    gsDeleteTexture(Texture_EffectLazerOrange);
}