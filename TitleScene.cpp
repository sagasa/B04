#include"TitleScene.h"

//�J�n
void TitleScene::start() {
	//�I���t���O�̏�����
	is_end_ = false;
	nextScene_ = "";
}

void TitleScene::update(float delta_time) {
	//�L�[�ŃV�[�����`�F���W
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;//�V�[���I��
		nextScene_ = "PlayerTestScene";
	} else if (gsGetKeyTrigger(GKEY_0))
	{
		is_end_ = true;
		nextScene_ = "EnemyTestScene";
	}
	else if (gsGetKeyTrigger(GKEY_9))
	{
		is_end_ = true;
		nextScene_ = "BossTestScene";
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
	return nextScene_;//���̃V�[������Ԃ�
}

//�I��
void TitleScene::end() {
	//���[���h������
	world_.clear();
}

//�w�i�̕`��
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{ scroll, 0.0f, scroll * 640.0f, 480.0f };
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}