#include"TitleScene.h"
#include"Assets.h"
#include<iostream>

//開始
void TitleScene::start() {
	fade_.start(true,3);
	//終了フラグの初期化
	is_end_ = false;
	nextScene_ = "";
	gsLoadTexture(Texture_TitleLogo, "Assets/Image/title_logo.dds");
	gsLoadTexture(Texture_push_button, "Assets/Image/push_button.dds");
	gsLoadTexture(Texture_Fade, "Assets/Image/black.dds");
}

void TitleScene::update(float delta_time) {
	fade_.update(delta_time);
	//キーでシーンをチェンジ
	if (gsGetKeyTrigger(GKEY_1)) {
		is_end_ = true;//シーン終了
		nextScene_ = "EnemyTestScene";
	}
	else if (gsGetKeyTrigger(GKEY_2))
	{
		is_end_ = true;
		nextScene_ = "BossTestScene";
	}
	if (gsGetKeyTrigger(GKEY_3))
	{
		is_end_ = true;
		nextScene_ = "PlayerTestScene";
	}
	if (gsGetKeyTrigger(GKEY_4) && fade_.is_end()) {
		is_end_ = true;
		nextScene_ = "GamePlayScene";
		fade_.is_change_fade_flg(true);
	}
	//背景のスクロール値を更新
	scroll_ += delta_time;
}

//描画
void TitleScene::draw() const {
	glPushMatrix();
	gsFontParameter(GS_FONT_BOLD, 16, "ＭＳ ゴシック");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("1 to EnemyTestScene");
	gsTextPos(0.0f, 40.0f);
	gsDrawText("2 to BossTestScene");
	gsTextPos(0.0f, 60.0f);
	gsDrawText("3 to PlayerTestScene");
	gsTextPos(0.0f, 80.0f);
	gsDrawText("4 to GamePlayScene");
	glPopMatrix();
	gsDrawSprite2D(Texture_TitleLogo, &GSvector2{ 300.0f,100.0f }, &GSrect{ 680.0f,160.0f }, &GSvector2{ 680.0f,160.0f }, NULL, NULL, 180.0f);

	if (fade_.is_end()) {
		gsDrawSprite2D(Texture_push_button, &GSvector2{ 400,500 }, &GSrect{ 940,145 }, &GSvector2{ 940,145 }, NULL, &GSvector2{ 0.5,0.5 }, 180);
	}
	fade_.draw();
}

//終了しているか？
bool TitleScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}else return false;//終了フラグを返す
}

//次のシーンを返す
std::string TitleScene::next() const {
	return nextScene_;//次のシーン名を返す
}

//終了
void TitleScene::end() {

}

//背景の描画
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{ scroll, 0.0f, scroll * 640.0f, 480.0f };
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}