#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>


//�J�n
void ResultScene::start() {
	is_end_ = false;
	select_ = Select::Next;
	//�e�N�X�`���̓ǂݍ���
	gsLoadTexture(Texture_Clear, "Assets/Image/clear.dds");
	gsLoadTexture(Texture_Return_To_Title, "Assets/Image/return_to_title.dds");
	gsLoadTexture(Texture_Next_Stage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_One_More, "Assets/Image/one_more.dds");
}

//�X�V
void ResultScene::update(float delta_time) {
	//���[���h�̍X�V
	//world_.update(delta_time);
	//�L�[����������^�C�g����
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}

}

//�`��
void ResultScene::draw() const {
	//���[���h�̕`��
	//world_.draw();
	GSvector2 position_clear{500.0f,50.0f};
	gsDrawSprite2D(Texture_Clear, &position_clear, NULL, NULL, NULL, NULL, NULL);
	GSvector2 position_next_stage{350.0f,250.0f};
	GSvector2 scale_next_stage{0.5f,0.5f};
	gsDrawSprite2D(Texture_Next_Stage, &position_next_stage, NULL, NULL, NULL, &scale_next_stage, NULL);
	GSvector2 position_one_more{450.0f,400.0f};
	GSvector2 scale_one_more{0.5f,0.5f};
	gsDrawSprite2D(Texture_One_More, &position_one_more, NULL, NULL, NULL, &scale_one_more, NULL);
	GSvector2 position_return_to_title{450.0f,550.0f};
	GSvector2 scale_return_to_title{0.5f,0.5f};
	gsDrawSprite2D(Texture_Return_To_Title, &position_return_to_title, NULL, NULL, NULL, &scale_return_to_title, NULL);
	
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
	gsDeleteTexture(Texture_Clear);
	gsDeleteTexture(Texture_Return_To_Title);
	gsDeleteTexture(Texture_Next_Stage);
	gsDeleteTexture(Texture_One_More);
}