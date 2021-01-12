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
	GSvector2 position_game_over{ 150.0f,30.0f };
	gsDrawSprite2D(Texture_Game_Over, &position_game_over, NULL, NULL, NULL, NULL, NULL);
	GSvector2 position_next_stage{ 350.0f,290.0f };
	GSvector2 scale_next_stage{ 0.5f,0.5f };
	gsDrawSprite2D(Texture_Next_Stage, &GSvector2{ 350,290 }, NULL, NULL, NULL, &scale_next_stage, NULL);
	GSvector2 position_one_more{ 450.0f,400.0f };
	GSvector2 scale_one_more{ 0.5f,0.5f };
	gsDrawSprite2D(Texture_One_More, &position_one_more, NULL, NULL, NULL, &scale_one_more, NULL);
	GSvector2 position_return_title{ 450.0f,550.0f };
	GSvector2 scale_return_title{0.5f,0.5f};
	gsDrawSprite2D(Texture_Return_To_Title, &position_return_title, NULL, NULL, NULL, &scale_return_title, NULL);
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
