#include"GamePlayScene.h"
#include"Player.h"
#include"EnemyGenerator.h"
#include"TextureID.h"

//開始
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
	//プレイヤーを追加
	world_.add_actor(new Player{ &world_,GSvector2{0.0f,240.0f} });
	//敵生成の追加
	world_.add_actor(new EnemyGenerator{ &world_ });
}

//更新
void GamePlayScene::update(float delta_time) {
	world_.update(delta_time);
}

//描画
void GamePlayScene::draw() const {
	world_.draw();
}

//終了しているか？
bool GamePlayScene::is_end() const {
	//ゲームオーバーかまたはゲームクリアならシーン終了
	return world_.is_game_over() || world_.is_game_clear();
}

//次のシーンを返す
std::string GamePlayScene::next() const {
	return "TitleScene";
}
//終了
void GamePlayScene::end() {
	//ワールドを消去
	world_.clear();
	//テクスチャの削除
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