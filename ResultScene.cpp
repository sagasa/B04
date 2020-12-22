#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>


//開始
void ResultScene::start() {
	is_end_ = false;
	//テクスチャの読み込み
}

//更新
void ResultScene::update(float delta_time) {
	//ワールドの更新
	world_.update(delta_time);
	//キーを押したらタイトルへ
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
}

//描画
void ResultScene::draw() const {
	//ワールドの描画
	world_.draw();
}

//終了しているか？
bool ResultScene::is_end() const {
	return is_end_;
}

//次のシーンを返す
std::string ResultScene::next() const {
	return "TitleScene";
}

//終了
void ResultScene::end() {

}