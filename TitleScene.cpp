#include"TitleScene.h"

//�J�n
void TitleScene::start() {
	//�I���t���O�̏�����
	is_end_ = false;
}

void TitleScene::update(float delta_time) {
	//�G���^�[�L�[����������V�[���I��
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;//�V�[���I��
	}
	//�w�i�̃X�N���[���l���X�V
	scroll_ += delta_time;
}

//�`��
void TitleScene::draw() const {
}

//�I�����Ă��邩�H
bool TitleScene::is_end() const {
	return is_end_;//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string TitleScene::next() const {
	return "GamePlayScene";//���̃V�[������Ԃ�
}

//�I��
void TitleScene::end() {
	
}

//�w�i�̕`��
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{scroll, 0.0f, scroll * 640.0f, 480.0f};
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}