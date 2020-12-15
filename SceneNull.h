#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include"IScene.h"

//ヌルシーン
class SceneNull : public IScene {
public:
	//開始
	void start() override {}
	//更新
	void update(float delta_time)override {}
	//描画
	void draw() const override {}
	//終了しているか？
	bool is_end() const override {
		return false;
	}
	//次のシーンを返す
	std::string next() const override {
		return "SceneNull";
	}
	//終了
	void end() override {}
};

#endif;