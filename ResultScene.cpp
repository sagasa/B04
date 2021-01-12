#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>


//���l�̑���l
const float Alpha_Value{ 0.016f };

//�J�n
void ResultScene::start() {
	fade_.start(true, 3);
	is_end_ = false;
	num_ = 0;
	alpha_flg_ = false;
	nextScene_ = "";
	//�e�N�X�`���̓ǂݍ���
	gsLoadTexture(Texture_Clear, "Assets/Image/clear.dds");
	gsLoadTexture(Texture_Return_To_Title, "Assets/Image/return_to_title.dds");
	gsLoadTexture(Texture_One_More, "Assets/Image/one_more.dds");
}

//�X�V
void ResultScene::update(float delta_time) {
	//�t�F�[�h�N���X�̍X�V
	fade_.update(delta_time);
	//���[���h�̍X�V
	//world_.update(delta_time);
	//�㉺�L�[�őI���ړ�
	if (gsGetKeyTrigger(GKEY_UP)) {
		--num_;
	}
	else if (gsGetKeyTrigger(GKEY_DOWN)) {
		++num_;
	}
	//F�L�[�Ō���
	if (gsGetKeyTrigger(GKEY_F)) {
		is_end_ = true;
		fade_.change_fade_flg();
		switch (num_)
		{
		case 0: nextScene_ = "GamePlayScene";break;
		case 1: nextScene_ = "TitleScene"; break;
		}
	}
	//�N�����v����
	num_ = CLAMP(num_, 0, 1);
	//���l�̍X�V
	update_alpha(num_, delta_time);
}

//�`��
void ResultScene::draw() const {
	//���[���h�̕`��
	//world_.draw();
	GSvector2 position_clear{ 500.0f,50.0f };
	gsDrawSprite2D(Texture_Clear, &position_clear, NULL, NULL, NULL, NULL, NULL);
	GSvector2 position_one_more{ 450.0f,400.0f };
	GSvector2 scale_one_more{ 0.5f,0.5f };
	GScolor color_one_more{ 1,1,1, alphas_[0] };
	gsDrawSprite2D(Texture_One_More, &position_one_more, NULL, NULL, &color_one_more, &scale_one_more, NULL);
	GSvector2 position_return_to_title{ 450.0f,550.0f };
	GSvector2 scale_return_to_title{ 0.5f,0.5f };
	GScolor color_return_to_title{ 1,1,1, alphas_[1] };
	gsDrawSprite2D(Texture_Return_To_Title, &position_return_to_title, NULL, NULL, &color_return_to_title, &scale_return_to_title, NULL);
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

//�I��
void ResultScene::end() {
	gsDeleteTexture(Texture_Clear);
	gsDeleteTexture(Texture_Return_To_Title);
	gsDeleteTexture(Texture_Next_Stage);
	gsDeleteTexture(Texture_One_More);
}

//���l�̍X�V
void ResultScene::update_alpha(int num, float delta_time) {
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