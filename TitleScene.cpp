#include"TitleScene.h"

//開始
void TitleScene::start() {
	//終了フラグの初期化
	is_end_ = false;
	nextScene_ = "";
}

void TitleScene::update(float delta_time) {
	//キーでシーンをチェンジ
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;//シーン終了
		nextScene_ = "PlayerTestScene";
	} else if (gsGetKeyTrigger(GKEY_0))
	{
		is_end_ = true;
		nextScene_ = "EnemyTestScene";
	}
	else if (gsGetKeyTrigger(GKEY_9))
	{
		is_end_ = true;
		nextScene_ = "BossTestScene";
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
	return nextScene_;//次のシーン名を返す
}

//終了
void TitleScene::end() {
	//ワールドを消去
	world_.clear();
}

//背景の描画
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{ scroll, 0.0f, scroll * 640.0f, 480.0f };
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}