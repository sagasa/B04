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
	gsLoadTexture(Texture_ReturnToTitle, "Assets/Image/return_to_title.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_OneMore, "Assets/Image/one_more.dds");

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
	gsSetMusicVolume(0.6f);
	//�t�F�[�h�N���X�̍X�V
	fade_.update(delta_time);
	//���[���h�̍X�V
	//world_.update(delta_time);
	if(fade_.is_end()) {
		//�㉺�L�[�őI���ړ�
		if (gsGetKeyTrigger(GKEY_UP)) {
			gsPlaySE(SE_Select);
			--num_;
		}
		else if (gsGetKeyTrigger(GKEY_DOWN)) {
			gsPlaySE(SE_Select);
			++num_;
		}
		//F�L�[�Ō���
		if (gsGetKeyTrigger(GKEY_F)) {
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
	GSvector2 position_game_over{ 150.0f,30.0f };
	gsDrawSprite2D(Texture_GameOver, &position_game_over, NULL, NULL, NULL, NULL, NULL);

	if (timer_ >= Time) {
		GSvector2 position_one_more{ 450.0f,400.0f };
		GSvector2 scale_one_more{ 0.5f,0.5f };
		GScolor color_one_more{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_OneMore, &position_one_more, NULL, NULL, &color_one_more, &scale_one_more, NULL);
		GSvector2 position_return_to_title{ 450.0f,550.0f };
		GSvector2 scale_return_to_title{ 0.5f,0.5f };
		GScolor color_return_to_title{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_ReturnToTitle, &position_return_to_title, NULL, NULL, &color_return_to_title, &scale_return_to_title, NULL);
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
	gsDeleteTexture(Texture_ReturnToTitle);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_OneMore);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//���l�̍X�V
void GameOverScene::update_alpha(int num, float delta_time) {
	if (num_ == 0) {
		//���l�̍X�V
		alphas_[0] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		alphas_[1] = 1.0f;
	}
	else if (num_ == 1) {
		//���l�̍X�V
		alphas_[1] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		alphas_[0] = 1.0f;
	}

	//�����t���O�𐧌�
	if (alphas_[num] >= 1.1f || alphas_[num] <= 0.0f) {
		alpha_flg_ = !alpha_flg_;
	}
}
