#include "GameOverScene.h"
#include"SceneManager.h"


//�J�n
void GameOverScene::start() {
	is_end_ = false;
}

//�X�V
void GameOverScene::update(float delta_time) {
	if (gsGetKeyTrigger(GKEY_RETURN)) {

	}
}

//�`��
void GameOverScene::draw() const {

}

//�I�����Ă��邩�H
bool GameOverScene::is_end() const {
	return is_end_;
}

//���̃V�[����Ԃ�
std::string GameOverScene::next() const{
	return "TitleScene";
}

//�I��
void GameOverScene::end() {

}
