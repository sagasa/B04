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
	gsDrawSprite2D(Texture_Clear, &GSvector2{ 500,50 }, NULL, NULL, NULL, NULL, NULL);
	gsDrawSprite2D(Texture_Next_Stage, &GSvector2{ 350,250 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
	gsDrawSprite2D(Texture_One_More, &GSvector2{ 450,400 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
	gsDrawSprite2D(Texture_Return_To_Title, &GSvector2{ 450,550 }, NULL, NULL, NULL, &GSvector2{0.5,0.5}, NULL);
	
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