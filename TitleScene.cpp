#include"TitleScene.h"

//�J�n
void TitleScene::start() {
	//�I���t���O�̏�����
	is_end_ = false;
	nextScene_ = "";
}

void TitleScene::update(float delta_time) {
	//�L�[�ŃV�[�����`�F���W
	if (gsGetKeyTrigger(GKEY_1)) {
		is_end_ = true;//�V�[���I��
		nextScene_ = "EnemyTestScene";
	}
	else if (gsGetKeyTrigger(GKEY_2))
	{
		is_end_ = true;
		nextScene_ = "BossTestScene";
	}
	if (gsGetKeyTrigger(GKEY_3))
	{
		is_end_ = true;
		nextScene_ = "PlayerTestScene";
	}
	//�w�i�̃X�N���[���l���X�V
	scroll_ += delta_time;
}

//�`��
void TitleScene::draw() const {

	gsFontParameter(GS_FONT_BOLD, 16, "�l�r �S�V�b�N");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("1 to EnemyTestScene");
	gsTextPos(0.0f, 40.0f);
	gsDrawText("2 to BossTestScene");
	gsTextPos(0.0f, 60.0f);
	gsDrawText("3 to PlayerTestScene");
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