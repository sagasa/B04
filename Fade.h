#ifndef FADE_H_
#define FADE_H_

#include <gslib.h>

//フェードクラス
class Fade {
public:
	//コンストラクタ
	Fade() = default;
	//デストラクタ
	~Fade();
	//開始(フェードインならtrue,フェードアウトならfalse),第二引数にフェードインorアウトしている時間
	void start(bool fade_in,float fade_time);
	//更新
	void update(float delta_time);
	//描画
	void draw() const;
	//フェードが終わったかを取得
	bool is_end() const;
	//ボタンが押されたか?
	void is_change_fade_flg(bool push_button);

private:
	//α値
	float color_alpha_;
	//α値に入れる値
	float alpha_value_;
	//フェードインかフェードアウトか？
	bool fade_in_;
	//フェードが終わったか？
	bool end_;
	//クリックされたか？
	bool change_fade_flg_;

};


#endif // !FADE_H_