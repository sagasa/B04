#include "StageClearScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>
#include<string>


//α値の代入値
const float Alpha_Value{ 0.016f };
//ボタンが出るまでの時間
const float Time{ 60.0f };
//配列の大きさ
const int Alphas_Size{ 2 };


//開始
void StageClearScene::start(int number) {
	//配列の初期化＆サイズ調整
	alphas_.resize(Alphas_Size, 1.0f);
	//終了フラグを初期化
	is_end_ = false;
	//ステージ番号を初期化
	stage_number_ = number;
	static_stage_number_ = 2;
	nextScene_ = "";
	//フェードクラスの開始
	fade_.start(true, 3);
	//テクスチャの読み込み
	gsLoadTexture(Texture_GameClear, "Assets/Image/gameclear.dds");
	gsLoadTexture(Texture_TitleButton, "Assets/Image/title_button.dds");
	gsLoadTexture(Texture_ContinueButton, "Assets/Image/continue_button.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_stageLogo, "Assets/Image/stagelogo.dds");
	gsLoadTexture(Texture_Number, "Assets/Image/number.dds");

	//BGMの読み込み
	gsLoadMusic(Music_GameClear, "Assets/BGM/gameclear.wav", GS_TRUE);
	gsBindMusic(Music_GameClear);
	gsPlayMusic();
	//SEの読み込み
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
}

//更新
void StageClearScene::update(float delta_time) {
	gsSetMusicVolume(0.8f);
	//フェードクラスの更新
	fade_.update(delta_time);
	//ワールドの更新
	//world_.update(delta_time);
	if (fade_.is_end()) {
		//キー入力、パッドの入力処理
		input();
		//α値の更新
		update_alpha(num_, delta_time);
		//タイマーの更新
		timer_ += delta_time;
	}
}

//描画
void StageClearScene::draw() const {
	//「ステージ」の描画
	GSvector2 position_stage{ 150.0f,30.0f };
	GSvector2 scale_stage{ 0.9f,0.9f };
	gsDrawSprite2D(Texture_stageLogo, &position_stage, NULL, NULL, NULL, &scale_stage, NULL);
	//ゲームクリアの描画
	GSvector2 position_clear{ 750.0f,30.0f };
	GSvector2 scale_clear{ 0.9f,0.9f };
	GSrect rect_clear{ 426,0, 852,200};
	gsDrawSprite2D(Texture_GameClear, &position_clear, &rect_clear, NULL, NULL, &scale_clear, NULL);
	//ボタンの描画
	draw_button();
	draw_number();
	//フェードクラスの描画
	fade_.draw();
}

//終了しているか?
bool StageClearScene::is_end() const{
	if (fade_.is_end()) {
		return is_end_;
	}
	else return false;//終了フラグを返す
}

//次のシーンを返す
std::string StageClearScene::next() const{
	return nextScene_;
}

//現在のステージ番号を返す
int StageClearScene::stage_number() const{
	return stage_number_;
}

//終了
void StageClearScene::end() {
	gsDeleteTexture(Texture_GameClear);
	gsDeleteTexture(Texture_TitleButton);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_ContinueButton);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//α値の更新
void StageClearScene::update_alpha(int num, float delta_time) {
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
void StageClearScene::input() {

	//左スティックのベクトル値
	GSvector2 vector_stick = GSvector2::zero();
	//左スティックの入力を取得
	gsXBoxPadGetLeftAxis(0, &vector_stick);
	//上下キーまたはパッドの十字ボタンまたは左スティックで選択移動
	if (num_ > 0 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_UP) || gsXBoxPadButtonState(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
		gsPlaySE(SE_Select);
		--num_;
	}
	else if (num_ < Alphas_Size - 1 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_DOWN) || gsXBoxPadButtonState(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
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
		case 0: 
			nextScene_ = "GamePlayScene";
			stage_number_++;
			break;
		case 1: nextScene_ = "TitleScene"; break;
		}
	}
	//クランプする
	num_ = CLAMP(num_, 0, Alphas_Size - 1);
}

//ボタンの描画
void StageClearScene::draw_button() const {

	if (timer_ >= Time) {
		GSvector2 position_nextstage_button{ 400.0f,300.0f };
		GSvector2 scale_nextstage_button{ 0.6f,0.6f };
		GScolor color_nextstage_button{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_NextStage, &position_nextstage_button, NULL, NULL, &color_nextstage_button, &scale_nextstage_button, NULL);
		GSvector2 position_title_button{ 500.0f,500.0f };
		GSvector2 scale_title_button{ 0.6f,0.6f };
		GScolor color_title_button{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_TitleButton, &position_title_button, NULL, NULL, &color_title_button, &scale_title_button, NULL);
	}
}

//番号の描画
void StageClearScene::draw_number() const {
	const GSvector2 position_number{520,30 };
	const GSrect rect_number{ 138 * (float)(static_stage_number_ - 1),0,138 * (float)static_stage_number_,200};
	gsDrawSprite2D(Texture_Number, &position_number, &rect_number, NULL, NULL, NULL, 0.0f);
}