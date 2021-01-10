#include"TitleScene.h"
#include"Assets.h"
#include<iostream>

//���l�̑���l
const float Alpha_Value{ 0.008f };
//�{�^�����o��܂ł̎���
const float Time{ 60.0f };

//�J�n
void TitleScene::start() {
	fade_.start(true,3);
	//�I���t���O�̏�����
	is_end_ = false;
	nextScene_ = "";
	gsLoadTexture(Texture_TitleLogo, "Assets/Image/title_logo.dds");
	gsLoadTexture(Texture_Push_Button, "Assets/Image/push_button.dds");
	gsLoadTexture(Texture_Fade, "Assets/Image/black.dds");
	alpha_flg_ = false;
	alpha_ = 0.0f;
}

void TitleScene::update(float delta_time) {
	fade_.update(delta_time);
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
	if (fade_.is_end()) {
		if (gsGetKeyTrigger(GKEY_F)) {
			is_end_ = true;
			nextScene_ = "GamePlayScene";
			fade_.change_fade_flg();
		}
		if (timer_ >= Time) {
			//���l�̍X�V
			alpha_ += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;

			if (alpha_ >= 1.1f || alpha_ <= 0.0f) {
				alpha_flg_ = !alpha_flg_;
			}
		}else timer_ += delta_time;//�^�C�}�[�X�V
		
	}
}

//�`��
void TitleScene::draw() const {
	glPushMatrix();
	gsFontParameter(GS_FONT_BOLD, 16, "�l�r �S�V�b�N");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("1 to EnemyTestScene");
	gsTextPos(0.0f, 40.0f);
	gsDrawText("2 to BossTestScene");
	gsTextPos(0.0f, 60.0f);
	gsDrawText("3 to PlayerTestScene");
	gsTextPos(0.0f, 80.0f);
	gsDrawText("4 to GamePlayScene");
	glPopMatrix();
	gsDrawSprite2D(Texture_TitleLogo, &GSvector2{ 300.0f,100.0f }, &GSrect{ 680.0f,160.0f }, &GSvector2{ 680.0f,160.0f }, NULL, NULL, 180.0f);

	if (timer_ >= Time) {
		gsDrawSprite2D(Texture_Push_Button, &GSvector2{ 400,500 }, &GSrect{ 940,145 }, &GSvector2{ 940,145 }, &GScolor{1,1,1,alpha_}, &GSvector2{ 0.5,0.5 }, 180);
	}
	fade_.draw();
}

//�I�����Ă��邩�H
bool TitleScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}else return false;//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string TitleScene::next() const {
	return nextScene_;//���̃V�[������Ԃ�
}

//�I��
void TitleScene::end() {
	timer_ = 0.0f;
}

//�w�i�̕`��
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{ scroll, 0.0f, scroll * 640.0f, 480.0f };
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}