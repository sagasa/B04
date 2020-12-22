#include "GameOverScene.h"
#include"SceneManager.h"


//開始
void GameOverScene::start() {
	is_end_ = false;
}

//更新
void GameOverScene::update(float delta_time) {
	if (gsGetKeyTrigger(GKEY_RETURN)) {

	}
}

//描画
void GameOverScene::draw() const {

}

//終了しているか？
bool GameOverScene::is_end() const {
	return is_end_;
}

//次のシーンを返す
std::string GameOverScene::next() const{
	return "TitleScene";
}

//終了
void GameOverScene::end() {

}
