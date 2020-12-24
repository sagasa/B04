#include "Fade.h"
#include"Assets.h"



//デストラクタ
Fade::~Fade() {
	//消去
	gsDeleteTexture(Texture_Fade);
}

//開始(フェードインならtrue,フェードアウトならfalse)
void Fade::start(bool fade_in,float fade_time) {
	//フェードインならtrue,フェードアウトならfalse
	fade_in_ = fade_in;
	//fa_in_の値で初期値を設定
	color_alpha_ = (fade_in_) ? 1.0f : 0.0f;
	//終了フラグを初期化
	end_ = false;
	//ボタン判定を初期化
	change_fade_flg_ = false;
	//α値に入れる値
	alpha_value_ = 0.016f / fade_time;
}

//更新
void Fade::update(float delta_time) {
	//α値の更新
	color_alpha_ += (fade_in_) ? -alpha_value_ * delta_time : alpha_value_ * delta_time;
	//クランプする
	color_alpha_ = CLAMP(color_alpha_, 0.0f, 1.1f);
	if (color_alpha_ >= 1.1f || color_alpha_ <= 0.0f) {
		end_ = true;
	}
}

//描画
void Fade::draw() const {

	gsDrawSprite2D(Texture_Fade, &GSvector2{ 0,0 }, NULL, NULL, &GScolor{ 1,1,1,color_alpha_ }, &GSvector2{ 10,6 }, NULL);
}

//フェードが終わったかを取得
bool Fade::is_end() const{
	return end_;
}

//ボタンが押されたか
void Fade::is_change_fade_flg(bool change_fade_flg) {
	change_fade_flg_ = change_fade_flg_;
	end_ = false;
	fade_in_ = false;
}
