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


//�J�n
void EnemyTestScene::start() {
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

    //�X�J�C�{�b�N�X�̓ǂݍ���
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
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
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //�G�l�~�[1
    world_.add_actor(new CarGhost{ &world_,GSvector3{10.0f,0.0f,0.0f} });
    //�G�l�~�[2
    world_.add_actor(new RushGhost{ &world_,GSvector3{-10.0f,0.0f,0.0f} });
    //�G�l�~�[3
    world_.add_actor(new Poltergeist{ &world_,GSvector3{5.0f,0.0f,0.0f} });
    //�G�l�~�[4
    world_.add_actor(new NormalGhost{ &world_,GSvector3{3.0f,5.0f,0.0f} });
    world_.add_actor(new SurogSakones{ &world_,GSvector3{15.0f,0.0f,0.0f} });
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
    world_.clear();
}