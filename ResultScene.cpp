#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>


//�J�n
void ResultScene::start() {
	is_end_ = false;
	//�e�N�X�`���̓ǂݍ���
}

//�X�V
void ResultScene::update(float delta_time) {
	//���[���h�̍X�V
	world_.update(delta_time);
	//�L�[����������^�C�g����
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
}

//�`��
void ResultScene::draw() const {
	//���[���h�̕`��
	world_.draw();
}

//�I�����Ă��邩�H
bool ResultScene::is_end() const {
	return is_end_;
}

//���̃V�[����Ԃ�
std::string ResultScene::next() const {
	return "TitleScene";
}

//�I��
void ResultScene::end() {

}