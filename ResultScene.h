#ifndef RESULT_SCENE_H_
#define RESULT_SCENE_H_

#include"IScene.h"
#include"World.h"
#include"Fade.h"

class ResultScene : public IScene
{
public:

	//開始
	virtual void start() override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end() const override;
	//次のシーンを返す
	virtual std::string next() const override;
	//終了
	virtual void end() override;

private:
	//α値の更新
	void update_alpha(int num, float detla_time);

private:
	//ワールドクラス
	World world_;
	//終了フラグ
	bool is_end_{ false };
	//α値の配列
	float alphas_[2] = {1.0f,1.0f};
	//配列識別番号
	int num_;
	//α値の増減フラグ
	bool alpha_flg_;
	//次のシーン
	std::string nextScene_;
	//フェードクラス
	Fade fade_;
	//タイマー
	float timer_;
};

#endif // !RESULT_SCENE_H_
