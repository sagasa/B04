#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include"IScene.h"
#include<gslib.h>
#include"World.h"
#include"Fade.h"
#include<vector>


//タイトルシーン
class TitleScene : public IScene {
public:

	//状態
	enum class State {
		Push_Button,
		Select,
		Tutorial
	};

	//開始
	virtual void start(int number) override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end() const override;
	//次のシーンを返す
	virtual std::string next() const override;
	//現在のステージ番号を返す
	virtual int stage_number() const override;
	//終了
	virtual void end() override;

private:
	void update_push_button(float delta_time);
	void update_select(float delta_time);
	void update_tutorial(float delta_time);
	//α値の更新
	void update_alpha(int num, float detla_time);

private:

	//終了フラグ
	bool is_end_{ false };
	//次のシーン
	std::string nextScene_;
	//フェードクラス
	Fade fade_;
	//ボタン表示用タイマー
	float button_timer_;
	//状態
	State state_;
	//α値の増減フラグ
	bool alpha_flg_;
	std::vector<float> alphas_;
	int select_;
	std::vector<float>pos_x_;
	int num_;
	bool is_move_;
	float move_timer_;
	float alpha_;

};

#endif
