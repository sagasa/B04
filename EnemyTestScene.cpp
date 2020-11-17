#include"EnemyTestScene.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Assets.h"
#include"SceneManager.h"


// ���[���h�N���X
World world_;
SceneManager scene_;

//�J�n
void EnemyTestScene::start() {
    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // �J�����̒ǉ�
    world_.add_camera(new Camera{ &world_ });
    // ���C�g�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���̒ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //�G�l�~�[1
    world_.add_actor(new CarGhost{ &world_,GSvector3{70.0f,30.0f,0.0f} });
    //�G�l�~�[2
    world_.add_actor(new RushGhost{ &world_,GSvector3{100.0f,0.0f,0.0f} });
    //�G�l�~�[3
    world_.add_actor(new Poltergeist{ &world_,GSvector3{70.0f,-50.0f,0.0f} });
}

//�X�V
void EnemyTestScene::update(float delta_time) {
    world_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_RETURN)) {
        scene_.end();
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