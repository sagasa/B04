#include "GameOverScene.h"
#include"SceneManager.h"
#include"Assets.h"


//�J�n
void GameOverScene::start() {
	is_end_ = false;
	
	//�e�N�X�`���̓ǂݍ���
	gsLoadTexture(Texture_Game_Over, "Assets/Image/gameover.dds");
	gsLoadTexture(Texture_Return_To_Title, "Assets/Image/return_to_title.dds");
	gsLoadTexture(Texture_Next_Stage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_One_More, "Assets/Image/one_more.dds");
}

//�X�V
void GameOverScene::update(float delta_time) {
	//world_.update(delta_time);
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
}

//�`��
void GameOverScene::draw() const {
	//world_.draw();
	gsDrawSprite2D(Texture_Game_Over, &GSvector2{ 150,30 }, NULL, NULL, NULL, NULL, NULL);
	gsDrawSprite2D(Texture_Next_Stage, &GSvector2{ 350,290 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
	gsDrawSprite2D(Texture_One_More, &GSvector2{ 450,400 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
	gsDrawSprite2D(Texture_Return_To_Title, &GSvector2{ 450,550 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
}

//�I�����Ă��邩�H
bool GameOverScene::is_end() const {
	return is_end_;
}

//���̃V�[����Ԃ�
std::string GameOverScene::next() const{
	return "TitleScene";
}

//�I��
void GameOverScene::end() {

}
