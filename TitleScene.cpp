#include"TitleScene.h"
#include"Assets.h"
#include<iostream>
#include<GSinput.h>

//α値の代入値
const float Alpha_Value{ 0.008f };
//ボタンが出るまでの時間
const float Time{ 60.0f };

//開始
void TitleScene::start() {
	fade_.start(true,3);
	//終了フラグの初期化
	is_end_ = false;
	nextScene_ = "";
	timer_ = 0;
	//テクスチャの読み込み
	gsLoadTexture(Texture_TitleLogo, "Assets/Image/title_logo.dds");
	gsLoadTexture(Texture_PushButton, "Assets/Image/push_button.dds");
	gsLoadTexture(Texture_Fade, "Assets/Image/black.dds");
	//BGMの読み込み
	gsLoadMusic(Music_Title, "Assets/BGM/title.wav", GS_TRUE);
	//SEの読み込み
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);

	alpha_flg_ = false;
	alpha_ = 0.0f;

	gsBindMusic(Music_Title);
	gsPlayMusic();
}

void TitleScene::update(float delta_time) {
	gsSetMusicVolume(0.8f);
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
	if (fade_.is_end()) {
		if (gsGetKeyTrigger(GKEY_F) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X)) {
			gsPlaySE(SE_Push);
			is_end_ = true;
			nextScene_ = "GamePlayScene";
			fade_.change_fade_flg();
		}
		if (timer_ >= Time) {
			//α値の更新
			alpha_ += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;

			if (alpha_ >= 1.1f || alpha_ <= 0.0f) {
				alpha_flg_ = !alpha_flg_;
			}
		}else timer_ += delta_time;//タイマー更新
	}
	
}

//描画
void TitleScene::draw() const {
#ifdef _DEBUG
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
#endif
	GSvector2 position_titlelogo{300.0f,100.0f};
	gsDrawSprite2D(Texture_TitleLogo, &position_titlelogo, NULL, NULL, NULL, NULL, NULL);

	if (timer_ >= Time) {
		GSvector2 position_push_to_button{400.0f,500.0f};
		GSvector2 scale_push_to_button{0.5f,0.5f};
		GScolor color{1,1,1,alpha_};
		gsDrawSprite2D(Texture_PushButton, &position_push_to_button,NULL, NULL, &color, &scale_push_to_button,NULL);
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
	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Push);
	gsDeleteTexture(Texture_TitleLogo);
	gsDeleteTexture(Texture_PushButton);
	timer_ = 0.0f;
}

//背景の描画
void TitleScene::draw_background(GSuint id, float scroll)const {
	GSrect rect{ scroll, 0.0f, scroll * 640.0f, 480.0f };
	gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}