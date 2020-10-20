#include"TitleScene.h"

//開始
void TitleScene::start() {
	//終了フラグの初期化
	is_end_ = false;
}

void TitleScene::update(float delta_time) {
	//エンターキーを押したらシーン終了
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;//シーン終了
	}
	//背景のスクロール値を更新
	scroll_ += delta_time;
}

//描画
void TitleScene::draw() const {
}

//終了しているか？
bool TitleScene::is_end() const {
	return is_end_;//終了フラグを返す
}

//次のシーンを返す
std::string TitleScene::next() const {
	return "GamePlayScene";//次のシーン名を返す
}

//終了
void TitleScene::end() {
	
}

//背景の描画
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{scroll, 0.0f, scroll * 640.0f, 480.0f};
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}