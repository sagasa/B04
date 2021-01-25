#include "GameOverScene.h"
#include"SceneManager.h"
#include"Assets.h"


//���l�̑���l
const float Alpha_Value{ 0.016f };
//�{�^�����o��܂ł̎���
const float Time{ 60.0f };

//�J�n
void GameOverScene::start() {
	fade_.start(true, 3);
	is_end_ = false;
	num_ = 0;
	alpha_flg_ = false;
	nextScene_ = "";
	timer_ = 0.0f;
	//�e�N�X�`���̓ǂݍ���
	gsLoadTexture(Texture_GameOver, "Assets/Image/gameover.dds");
	gsLoadTexture(Texture_TitleButton, "Assets/Image/title_button.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_RestartButton, "Assets/Image/restart_button.dds");

	//BGM�̓ǂݍ���
	gsLoadMusic(Music_GameOver, "Assets/BGM/gameover.wav", GS_TRUE);
	gsBindMusic(Music_GameOver);
	gsPlayMusic();
	//SE�̓ǂݍ���
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
}

//�X�V
void GameOverScene::update(float delta_time) {
	//�I���̍ő吔
	const float Max_Select{ sizeof(alphas_) / sizeof(*alphas_) };
	gsSetMusicVolume(0.8f);
	//�t�F�[�h�N���X�̍X�V
	fade_.update(delta_time);
	//���[���h�̍X�V
	//world_.update(delta_time);
	if(fade_.is_end()) {
		//���X�e�B�b�N�̃x�N�g���l
		GSvector2 vector_stick = GSvector2::zero();
		//���X�e�B�b�N�̓��͂��擾
		gsXBoxPadGetLeftAxis(0, &vector_stick);
		//�㉺�L�[�܂��̓p�b�h�̏\���{�^���܂��͍��X�e�B�b�N�őI���ړ�
		if (num_ > 0 && (gsGetKeyTrigger(GKEY_UP) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
			gsPlaySE(SE_Select);
			--num_;
		}
		else if (num_ < Max_Select - 1 && (gsGetKeyTrigger(GKEY_DOWN) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
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
			case 0: nextScene_ = "GamePlayScene"; break;
			case 1: nextScene_ = "TitleScene"; break;
			}
		}
		//�N�����v����
		num_ = CLAMP(num_, 0, 1);
		//���l�̍X�V
		update_alpha(num_, delta_time);
		
		timer_ += delta_time;
	}
	
}

//�`��
void GameOverScene::draw() const {
	//world_.draw();
	GSvector2 position_game_over{ 250.0f,30.0f };
	gsDrawSprite2D(Texture_GameOver, &position_game_over, NULL, NULL, NULL, NULL, NULL);

	if (timer_ >= Time) {
		GSvector2 position_restart_button{ 450.0f,300.0f };
		GSvector2 scale_restart_button{ 0.7f,0.7f };
		GScolor color_restart_button{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_RestartButton, &position_restart_button, NULL, NULL, &color_restart_button, &scale_restart_button, NULL);
		GSvector2 position_return_to_title{ 480.0f,500.0f };
		GSvector2 scale_return_to_title{ 0.7f,0.7f };
		GScolor color_return_to_title{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_TitleButton, &position_return_to_title, NULL, NULL, &color_return_to_title, &scale_return_to_title, NULL);
	}
	
	//�t�F�[�h�N���X�̕`��
	fade_.draw();
}

//�I�����Ă��邩�H
bool GameOverScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}
	else return false;//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string GameOverScene::next() const{
	return nextScene_;
}

//�I��
void GameOverScene::end() {
	gsDeleteTexture(Texture_GameOver);
	gsDeleteTexture(Texture_TitleButton);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_RestartButton);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//���l�̍X�V
void GameOverScene::update_alpha(int num, float delta_time) {
	for (int i = 0; i < sizeof(alphas_) / sizeof(*alphas_); ++i) {
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
