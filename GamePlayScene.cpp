#include"GamePlayScene.h"
#include"Player.h"
#include"EnemyGenerator.h"

//�J�n
void GamePlayScene::start() {
	
	//�v���C���[��ǉ�
	world_.add_actor(new Player{ &world_,GSvector2{0.0f,240.0f} });
}

//�X�V
void GamePlayScene::update(float delta_time) {
	world_.update(delta_time);
}

//�`��
void GamePlayScene::draw() const {
	world_.draw();
}

//�I�����Ă��邩�H
bool GamePlayScene::is_end() const {
	return false;
}

//���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
	return "TitleScene";
}
//�I��
void GamePlayScene::end() {
	//���[���h������
	world_.clear();
	//�e�N�X�`���̍폜
	
}