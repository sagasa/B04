#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>
#include<iostream>


//α値の代入値
const float Alpha_Value{ 0.016f };
//ボタンが出るまでの時間
const float Time{ 60.0f };

//開始
void ResultScene::start() {
	fade_.start(true, 3);
	is_end_ = false;
	num_ = 0;
	alpha_flg_ = false;
	nextScene_ = "";
	timer_ = 0;
	//テクスチャの読み込み
	gsLoadTexture(Texture_GameClear, "Assets/Image/gameclear.dds");
	gsLoadTexture(Texture_TitleButton, "Assets/Image/title_button.dds");
	gsLoadTexture(Texture_RestartButton, "Assets/Image/restart_button.dds");

	//BGMの読み込み
	gsLoadMusic(Music_GameClear, "Assets/BGM/gameclear.wav", GS_TRUE);
	gsBindMusic(Music_GameClear);
	gsPlayMusic();
	//SEの読み込み
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
}

//更新
void ResultScene::update(float delta_time) {
	//選択の最大数
	const float Max_Select{ sizeof(alphas_) / sizeof(*alphas_) };
	gsSetMusicVolume(0.8f);
	//フェードクラスの更新
	fade_.update(delta_time);
	//ワールドの更新
	//world_.update(delta_time);
	if (fade_.is_end()) {
		//左スティックのベクトル値
		GSvector2 vector_stick = GSvector2::zero();
		//左スティックの入力を取得
		gsXBoxPadGetLeftAxis(0, &(vector_stick));
		//上下キーまたはパッドの十字ボタンまたは左スティックで選択移動
		if (num_ > 0 && (gsGetKeyTrigger(GKEY_UP) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
			gsPlaySE(SE_Select);
			--num_;
		}
		else if (num_ < Max_Select - 1 && (gsGetKeyTrigger(GKEY_DOWN) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
			gsPlaySE(SE_Select);
			++num_;
		}
		//FキーまたはXボタンで決定
		if (gsGetKeyTrigger(GKEY_F) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X)) {
			gsPlaySE(SE_Push);
			is_end_ = true;
			fade_.change_fade_flg();
			switch (num_)
			{
			case 0: nextScene_ = "GamePlayScene"; break;
			case 1: nextScene_ = "TitleScene"; break;
			}
		}
		//クランプする
		num_ = CLAMP(num_, 0, 1);
		//α値の更新
		update_alpha(num_, delta_time);
		timer_ += delta_time;

	}
}

//描画
void ResultScene::draw() const {
	//ワールドの描画
	//world_.draw();
	GSvector2 position_clear{ 226.0f,50.0f };
	gsDrawSprite2D(Texture_GameClear, &position_clear, NULL, NULL, NULL, NULL, NULL);
	if (timer_ >= Time) {
		GSvector2 position_restart_button{ 450.0f,300.0f };
		GSvector2 scale_restart_button{ 0.7f,0.7f };
		GScolor color_restart_button{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_RestartButton, &position_restart_button, NULL, NULL, &color_restart_button, &scale_restart_button, NULL);
		GSvector2 position_return_to_title{ 480.0f,500.0f };
		GSvector2 scale_return_to_title{ 0.7f,0.7f };
		GScolor color_return_to_title{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_TitleButton, &position_return_to_title, NULL, NULL, &color_return_to_title, &scale_return_to_title, NULL);
	}

	//フェードクラスの描画
	fade_.draw();
}

//終了しているか？
bool ResultScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}
	else return false;//終了フラグを返す
}

//次のシーンを返す
std::string ResultScene::next() const {
	return nextScene_;
}

//終了
void ResultScene::end() {
	gsDeleteTexture(Texture_GameClear);
	gsDeleteTexture(Texture_TitleButton);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_RestartButton);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//α値の更新
void ResultScene::update_alpha(int num, float delta_time) {
	//α値の更新処理
	for (int i = 0; i < sizeof(alphas_) / sizeof(*alphas_); ++i) {
		if (i == num) {
			//α値の更新
			alphas_[i] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		}
		else alphas_[i] = 1.0f;
	}

	//増減フラグを制御
	if (alphas_[num] >= 1.1f || alphas_[num] <= 0.0f) {
		alpha_flg_ = !alpha_flg_;
	}
}