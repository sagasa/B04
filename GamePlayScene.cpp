#include"GamePlayScene.h"
#include"Player.h"
#include"EnemyGenerator.h"

//開始
void GamePlayScene::start() {
	
	//プレイヤーを追加
	world_.add_actor(new Player{ &world_,GSvector2{0.0f,240.0f} });
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
	return false;
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
	
}