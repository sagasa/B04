#include "GameOverScene.h"
#include"SceneManager.h"
#include"Assets.h"


//α値の代入値
const float Alpha_Value{ 0.016f };
//ボタンが出るまでの時間
const float Time{ 60.0f };
//サイズ
const int Alphas_Size{2};

//開始
void GameOverScene::start(int number) {
	fade_.start(true, 3);
	is_end_ = false;
	num_ = 0;
	alpha_flg_ = false;
	nextScene_ = "";
	timer_ = 0.0f;
	alphas_.resize(Alphas_Size, 1.0f);
	stage_number_ = number;
	//テクスチャの読み込み
	gsLoadTexture(Texture_GameOver, "Assets/Image/gameover.dds");
	gsLoadTexture(Texture_TitleButton, "Assets/Image/title_button.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_ContinueButton, "Assets/Image/continue_button.dds");

	//BGMの読み込み
	gsLoadMusic(Music_GameOver, "Assets/BGM/gameover.wav", GS_TRUE);
	gsBindMusic(Music_GameOver);
	gsPlayMusic();
	//SEの読み込み
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
}

//更新
void GameOverScene::update(float delta_time) {
	gsSetMusicVolume(0.8f);
	//フェードクラスの更新
	fade_.update(delta_time);
	//ワールドの更新
	//world_.update(delta_time);
	if(fade_.is_end()) {
		//キー入力、パッドの入力処理
		input();
		//α値の更新
		update_alpha(num_, delta_time);
		//タイマーの更新
		timer_ += delta_time;
	}
	
}

//描画
void GameOverScene::draw() const {
	//world_.draw();
	//ゲームオーバーを描画
	GSvector2 position_game_over{ 250.0f,30.0f };
	gsDrawSprite2D(Texture_GameOver, &position_game_over, NULL, NULL, NULL, NULL, NULL);
	//ボタンの描画
	draw_button();
	//フェードクラスの描画
	fade_.draw();
}

//終了しているか？
bool GameOverScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}
	else return false;//終了フラグを返す
}

//次のシーンを返す
std::string GameOverScene::next() const{
	return nextScene_;
}

//現在のステージ番号を返す
int GameOverScene::stage_number() const {
	return stage_number_;
}

//終了
void GameOverScene::end() {
	gsDeleteTexture(Texture_GameOver);
	gsDeleteTexture(Texture_TitleButton);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_ContinueButton);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//α値の更新
void GameOverScene::update_alpha(int num, float delta_time) {
	for (int i = 0; i < Alphas_Size; ++i) {
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

//キー入力、パッドの入力処理
void GameOverScene::input() {

	//左スティックのベクトル値
	GSvector2 vector_stick = GSvector2::zero();
	//左スティックの入力を取得
	gsXBoxPadGetLeftAxis(0, &vector_stick);
	//上下キーまたはパッドの十字ボタンまたは左スティックで選択移動
	if (num_ > 0 && !gsIsPlaySE(SE_Select)&& (gsGetKeyState(GKEY_UP) || gsXBoxPadButtonState(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
		gsPlaySE(SE_Select);
		--num_;
	}
	else if (num_ < Alphas_Size - 1&& !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_DOWN) || gsXBoxPadButtonState(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
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
	num_ = CLAMP(num_, 0, Alphas_Size - 1);
}

//ボタンの描画
void GameOverScene::draw_button() const {

	if (timer_ >= Time) {
		GSvector2 position_continue_button{ 400.0f,300.0f };
		GSvector2 scale_continue_button{ 0.7f,0.7f };
		GScolor color_continue_button{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_ContinueButton, &position_continue_button, NULL, NULL, &color_continue_button, &scale_continue_button, NULL);
		GSvector2 position_return_to_title{ 480.0f,500.0f };
		GSvector2 scale_return_to_title{ 0.7f,0.7f };
		GScolor color_return_to_title{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_TitleButton, &position_return_to_title, NULL, NULL, &color_return_to_title, &scale_return_to_title, NULL);
	}
}
