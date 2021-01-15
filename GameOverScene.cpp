#include "GameOverScene.h"
#include"SceneManager.h"
#include"Assets.h"


//α値の代入値
const float Alpha_Value{ 0.016f };
//ボタンが出るまでの時間
const float Time{ 60.0f };

//開始
void GameOverScene::start() {
	fade_.start(true, 3);
	is_end_ = false;
	num_ = 0;
	alpha_flg_ = false;
	nextScene_ = "";
	timer_ = 0.0f;
	//テクスチャの読み込み
	gsLoadTexture(Texture_GameOver, "Assets/Image/gameover.dds");
	gsLoadTexture(Texture_ReturnToTitle, "Assets/Image/return_to_title.dds");
	gsLoadTexture(Texture_NextStage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_OneMore, "Assets/Image/one_more.dds");

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
	gsSetMusicVolume(0.6f);
	//フェードクラスの更新
	fade_.update(delta_time);
	//ワールドの更新
	//world_.update(delta_time);
	if(fade_.is_end()) {
		//上下キーで選択移動
		if (gsGetKeyTrigger(GKEY_UP)) {
			gsPlaySE(SE_Select);
			--num_;
		}
		else if (gsGetKeyTrigger(GKEY_DOWN)) {
			gsPlaySE(SE_Select);
			++num_;
		}
		//Fキーで決定
		if (gsGetKeyTrigger(GKEY_F)) {
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
void GameOverScene::draw() const {
	//world_.draw();
	GSvector2 position_game_over{ 150.0f,30.0f };
	gsDrawSprite2D(Texture_GameOver, &position_game_over, NULL, NULL, NULL, NULL, NULL);

	if (timer_ >= Time) {
		GSvector2 position_one_more{ 450.0f,400.0f };
		GSvector2 scale_one_more{ 0.5f,0.5f };
		GScolor color_one_more{ 1,1,1, alphas_[0] };
		gsDrawSprite2D(Texture_OneMore, &position_one_more, NULL, NULL, &color_one_more, &scale_one_more, NULL);
		GSvector2 position_return_to_title{ 450.0f,550.0f };
		GSvector2 scale_return_to_title{ 0.5f,0.5f };
		GScolor color_return_to_title{ 1,1,1, alphas_[1] };
		gsDrawSprite2D(Texture_ReturnToTitle, &position_return_to_title, NULL, NULL, &color_return_to_title, &scale_return_to_title, NULL);
	}
	
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

//終了
void GameOverScene::end() {
	gsDeleteTexture(Texture_GameOver);
	gsDeleteTexture(Texture_ReturnToTitle);
	gsDeleteTexture(Texture_NextStage);
	gsDeleteTexture(Texture_OneMore);

	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_Push);
}

//α値の更新
void GameOverScene::update_alpha(int num, float delta_time) {
	if (num_ == 0) {
		//α値の更新
		alphas_[0] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		alphas_[1] = 1.0f;
	}
	else if (num_ == 1) {
		//α値の更新
		alphas_[1] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		alphas_[0] = 1.0f;
	}

	//増減フラグを制御
	if (alphas_[num] >= 1.1f || alphas_[num] <= 0.0f) {
		alpha_flg_ = !alpha_flg_;
	}
}
