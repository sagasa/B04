#ifndef STAGE_CLEAR_SCENE
#define STAGE_CLEAR_SCENE

#include"IScene.h"
#include"World.h"
#include"Fade.h"


class StageClearScene : public IScene
{
public:
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
	//α値の更新
	void update_alpha(int num, float detla_time);
	//キー入力、パッドの入力処理
	void input();
	//ボタンの描画
	void draw_button() const;
	//数字の描画
	void draw_number() const;
private:
	//ワールドクラス
	World world_;
	//終了フラグ
	bool is_end_{ false };
	//各テクスチャのα値のデータ
	std::vector<float> alphas_;
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
	//ステージ名を出すためのタイマー
	float draw_timer_;
	//ステージ番号を保存する変数
	int static_stage_number_;

};



#endif // !STAGE_CLEAR_SCENE



