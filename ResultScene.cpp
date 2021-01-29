#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>
#include<iostream>
#include<fstream>


//���l�̑���l
const float Alpha_Value{ 0.016f };
//�{�^�����o��܂ł̎���
const float Time{ 60.0f };
//�z��̑傫��
const int Alphas_Size{3};

//�J�n
void ResultScene::start(int number) {
	fade_.start(true, 3);
	is_end_ = false;
	num_ = 0;
	alpha_flg_ = false;
	nextScene_ = "";
	timer_ = 0;
	alphas_.resize(Alphas_Size, 1.0f);
	stage_number_ = number;
	//�e�N�X�`���̓ǂݍ���
	gsLoadTexture(Texture_GameClear, "Assets/Image/gameclear.dds");
	gsLoadTexture(Texture_TitleButton, "Assets/Image/title_button.dds");
	gsLoadTexture(Texture_ContinueButton, "Assets/Image/continue_button.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");

	//BGM�̓ǂݍ���
	gsLoadMusic(Music_GameClear, "Assets/BGM/gameclear.wav", GS_TRUE);
	gsBindMusic(Music_GameClear);
	gsPlayMusic();
	//SE�̓ǂݍ���
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
}

//�X�V
void ResultScene::update(float delta_time) {
	//�I���̍ő吔
	//const float Max_Select{ sizeof(alphas_) / sizeof(*alphas_) };
	gsSetMusicVolume(0.8f);
	//�t�F�[�h�N���X�̍X�V
	fade_.update(delta_time);
	//���[���h�̍X�V
	//world_.update(delta_time);
	if (fade_.is_end()) {
		//�L�[���́A�p�b�h�̓��͏���
		input();
		//���l�̍X�V
		update_alpha(num_, delta_time);
		//�^�C�}�[�̍X�V
		timer_ += delta_time;

	}
}

//�`��
void ResultScene::draw() const {
	//���[���h�̕`��
	//world_.draw();
	//�Q�[���N���A�̕`��
	GSvector2 position_clear{ 250.0f,30.0f };
	GSvector2 scale_clear{ 0.9f,0.9f };
	gsDrawSprite2D(Texture_GameClear, &position_clear, NULL, NULL, NULL, &scale_clear, NULL);
	//�{�^���̕`��
	draw_button();
	//�t�F�[�h�N���X�̕`��
	fade_.draw();
}

//�I�����Ă��邩�H
bool ResultScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}
	else return false;//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string ResultScene::next() const {
	return nextScene_;
}

//���݂̃X�e�[�W�ԍ���Ԃ�
int ResultScene::stage_number() const {
	return stage_number_;
}

//�I��
void ResultScene::end() {
	gsDeleteTexture(Texture_GameClear);
	gsDeleteTexture(Texture_TitleButton);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_ContinueButton);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//���l�̍X�V
void ResultScene::update_alpha(int num, float delta_time) {
	//���l�̍X�V����
	for (int i = 0; i < Alphas_Size; ++i) {
		if (i == num) {
			//���l�̍X�V
			alphas_[i] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		}
		else alphas_[i] = 1.0f;
	}

	//�����t���O�𐧌�
	if (alphas_[num] >= 1.1f || alphas_[num] <= 0.0f) {
		alpha_flg_ = !alpha_flg_;
	}
}

//�L�[���́A�p�b�h�̓��͏����̍X�V
void ResultScene::input() {
	//���X�e�B�b�N�̃x�N�g���l
	GSvector2 vector_stick = GSvector2::zero();
	//���X�e�B�b�N�̓��͂��擾
	gsXBoxPadGetLeftAxis(0, &(vector_stick));
	//�㉺�L�[�܂��̓p�b�h�̏\���{�^���܂��͍��X�e�B�b�N�őI���ړ�
	if (num_ > 0 && (gsGetKeyTrigger(GKEY_UP) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
		gsPlaySE(SE_Select);
		--num_;
	}
	else if (num_ < Alphas_Size - 1 && (gsGetKeyTrigger(GKEY_DOWN) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
		gsPlaySE(SE_Select);
		++num_;
	}
	//F�L�[�܂���X�{�^���Ō���
	if (gsGetKeyTrigger(GKEY_F) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X)) {
		gsPlaySE(SE_Push);
		is_end_ = true;
		fade_.change_fade_flg();
		switch (num_)
		{
		case 0:
			nextScene_ = "GamePlayScene";
			stage_number_++;
			break;
		case 1: nextScene_ = "GamePlayScene"; break;
		case 2: nextScene_ = "TitleScene"; break;
		}
	}
	//�N�����v����
	num_ = CLAMP(num_, 0, Alphas_Size - 1);
}

//�{�^���̕`��
void ResultScene::draw_button() const {
	
	if (timer_ >= Time) {
		GSvector2 position_nextstage_button{ 400.0f,250.0f };
		GSvector2 scale_nextstage_button{ 0.6f,0.6f };
		GScolor color_nextstage_button{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_NextStage, &position_nextstage_button, NULL, NULL, &color_nextstage_button, &scale_nextstage_button, NULL);
		GSvector2 position_continue_button{ 430.0f,420.0f };
		GSvector2 scale_continue_button{ 0.6f,0.6f };
		GScolor color_continue_button{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_ContinueButton, &position_continue_button, NULL, NULL, &color_continue_button, &scale_continue_button, NULL);
		GSvector2 position_title_button{ 500.0f,550.0f };
		GSvector2 scale_title_button{ 0.6f,0.6f };
		GScolor color_title_button{ 1,1,1, alphas_[2] };
		gsDrawSprite2D(Texture_TitleButton, &position_title_button, NULL, NULL, &color_title_button, &scale_title_button, NULL);
	}
}