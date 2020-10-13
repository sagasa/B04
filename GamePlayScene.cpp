#include"GamePlayScene.h"
#include"Player.h"
#include"EnemyGenerator.h"
#include"TextureID.h"

//�J�n
void GamePlayScene::start() {
	gsLoadTexture(TexturePlayer, "Assets/SHIP.png");
	gsLoadTexture(TextureEnemy, "Assets/ENEMY.png");
	gsLoadTexture(TextureEnemy2, "Assets/ENEMY2.png");
	gsLoadTexture(TextureBG1, "Assets/BG1.png");
	gsLoadTexture(TextureBG2, "Assets/BG2.png");
	gsLoadTexture(TextureBG3, "Assets/BG3.png");
	gsLoadTexture(TexturePlayerBeam, "Assets/BEAM.png");
	gsLoadTexture(TextureEnemyBeam, "Assets/EBEAM.pmg");
	gsLoadTexture(TextureBomb, "Assets/BOMB.png");
	gsLoadTexture(TextureNumber, "Assets/NUM.png");
	//�v���C���[��ǉ�
	world_.add_actor(new Player{ &world_,GSvector2{0.0f,240.0f} });
	//�G�����̒ǉ�
	world_.add_actor(new EnemyGenerator{ &world_ });
}

//�X�V
void GamePlayScene::update(float delta_time) {
	world_.update(delta_time);
}

//�`��
void GamePlayScene::draw() const {
	world_.draw();
}

//�I�����Ă��邩�H
bool GamePlayScene::is_end() const {
	//�Q�[���I�[�o�[���܂��̓Q�[���N���A�Ȃ�V�[���I��
	return world_.is_game_over() || world_.is_game_clear();
}

//���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
	return "TitleScene";
}
//�I��
void GamePlayScene::end() {
	//���[���h������
	world_.clear();
	//�e�N�X�`���̍폜
	gsDeleteTexture(TexturePlayer);
	gsDeleteTexture(TextureEnemy);
	gsDeleteTexture(TextureEnemy2);
	gsDeleteTexture(TexturePlayerBeam);
	gsDeleteTexture(TextureEnemyBeam);
	gsDeleteTexture(TextureBG1);
	gsDeleteTexture(TextureBG2);
	gsDeleteTexture(TextureBG3);
	gsDeleteTexture(TextureBomb);
	gsDeleteTexture(TextureNumber);
}