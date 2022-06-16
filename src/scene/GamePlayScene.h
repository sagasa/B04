#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include"IScene.h"
#include"World.h"
#include"Fade.h"

//ゲームプレイシーン
class GamePlayScene :public IScene {
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
	//ワールドクラス
	World world_;
	//終了フラグ
	bool is_end_{ false };
	//次のシーン
	std::string next_scene_{"SceneNull"};
	//フェードクラス
	Fade fade_;
	//ステージ名を出すためのタイマー
	float draw_timer_;
};

#endif
