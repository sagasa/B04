#include "StageClearScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>
#include<string>


//���l�̑���l
const float Alpha_Value{ 0.016f };
//�{�^�����o��܂ł̎���
const float Time{ 60.0f };
//�z��̑傫��
const int Alphas_Size{ 2 };


//�J�n
void StageClearScene::start(int number) {
	//�z��̏��������T�C�Y����
	alphas_.resize(Alphas_Size, 1.0f);
	//�I���t���O��������
	is_end_ = false;
	//�X�e�[�W�ԍ���������
	stage_number_ = number;
	static_stage_number_ = 2;
	nextScene_ = "";
	//�t�F�[�h�N���X�̊J�n
	fade_.start(true, 3);
	//�e�N�X�`���̓ǂݍ���
	gsLoadTexture(Texture_GameClear, "Assets/Image/gameclear.dds");
	gsLoadTexture(Texture_TitleButton, "Assets/Image/title_button.dds");
	gsLoadTexture(Texture_ContinueButton, "Assets/Image/continue_button.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_stageLogo, "Assets/Image/stagelogo.dds");
	gsLoadTexture(Texture_Number, "Assets/Image/number.dds");

	//BGM�̓ǂݍ���
	gsLoadMusic(Music_GameClear, "Assets/BGM/gameclear.wav", GS_TRUE);
	gsBindMusic(Music_GameClear);
	gsPlayMusic();
	//SE�̓ǂݍ���
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
}

//�X�V
void StageClearScene::update(float delta_time) {
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
void StageClearScene::draw() const {
	//�u�X�e�[�W�v�̕`��
	GSvector2 position_stage{ 150.0f,30.0f };
	GSvector2 scale_stage{ 0.9f,0.9f };
	gsDrawSprite2D(Texture_stageLogo, &position_stage, NULL, NULL, NULL, &scale_stage, NULL);
	//�Q�[���N���A�̕`��
	GSvector2 position_clear{ 750.0f,30.0f };
	GSvector2 scale_clear{ 0.9f,0.9f };
	GSrect rect_clear{ 426,0, 852,200};
	gsDrawSprite2D(Texture_GameClear, &position_clear, &rect_clear, NULL, NULL, &scale_clear, NULL);
	//�{�^���̕`��
	draw_button();
	draw_number();
	//�t�F�[�h�N���X�̕`��
	fade_.draw();
}

//�I�����Ă��邩?
bool StageClearScene::is_end() const{
	if (fade_.is_end()) {
		return is_end_;
	}
	else return false;//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string StageClearScene::next() const{
	return nextScene_;
}

//���݂̃X�e�[�W�ԍ���Ԃ�
int StageClearScene::stage_number() const{
	return stage_number_;
}

//�I��
void StageClearScene::end() {
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
void StageClearScene::update_alpha(int num, float delta_time) {
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

//�L�[���́A�p�b�h�̓��͏���
void StageClearScene::input() {

	//���X�e�B�b�N�̃x�N�g���l
	GSvector2 vector_stick = GSvector2::zero();
	//���X�e�B�b�N�̓��͂��擾
	gsXBoxPadGetLeftAxis(0, &vector_stick);
	//�㉺�L�[�܂��̓p�b�h�̏\���{�^���܂��͍��X�e�B�b�N�őI���ړ�
	if (num_ > 0 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_UP) || gsXBoxPadButtonState(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
		gsPlaySE(SE_Select);
		--num_;
	}
	else if (num_ < Alphas_Size - 1 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_DOWN) || gsXBoxPadButtonState(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
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
		case 1: nextScene_ = "TitleScene"; break;
		}
	}
	//�N�����v����
	num_ = CLAMP(num_, 0, Alphas_Size - 1);
}

//�{�^���̕`��
void StageClearScene::draw_button() const {

	if (timer_ >= Time) {
		GSvector2 position_nextstage_button{ 400.0f,300.0f };
		GSvector2 scale_nextstage_button{ 0.6f,0.6f };
		GScolor color_nextstage_button{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_NextStage, &position_nextstage_button, NULL, NULL, &color_nextstage_button, &scale_nextstage_button, NULL);
		GSvector2 position_title_button{ 500.0f,500.0f };
		GSvector2 scale_title_button{ 0.6f,0.6f };
		GScolor color_title_button{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_TitleButton, &position_title_button, NULL, NULL, &color_title_button, &scale_title_button, NULL);
	}
}

//�ԍ��̕`��
void StageClearScene::draw_number() const {
	const GSvector2 position_number{520,30 };
	const GSrect rect_number{ 138 * (float)(static_stage_number_ - 1),0,138 * (float)static_stage_number_,200};
	gsDrawSprite2D(Texture_Number, &position_number, &rect_number, NULL, NULL, NULL, 0.0f);
}