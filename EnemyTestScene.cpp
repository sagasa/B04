#include"EnemyTestScene.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Assets.h"


// ���[���h�N���X
World world_;

//�J�n
void EnemyTestScene::start() {
    // �t�B�[���h�̒ǉ�
    world_.add_field(new Field{ Texture_BgTileNebulaGreen });
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
}

//�`��
void EnemyTestScene::draw() const {
    world_.draw();
}

//�I�����Ă��邩
bool EnemyTestScene::is_end() const {
	return false;
}

//���̃V�[����Ԃ�
/*std::string EnemyTestScene::next() const {

}*/

//�I��
void EnemyTestScene::end() {
    world_.clear();
    // ���b�V���̍폜
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_CarGhost);
    gsDeleteMesh(Mesh_RushGhost);
    gsDeleteMesh(Mesh_Poltergeist);
}