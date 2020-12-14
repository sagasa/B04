#ifndef PLAYER_TEST_SCENE_H_
#define PLAYER_TEST_SCENE_H_

#include "IScene.h"
#include"World.h"
class PlayerTestScene :
	public IScene
{
	void load(resource_loader& loader) override;
	//開始
	virtual void start() override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end() const override;
	//次の市0ン名を返す
	virtual std::string next() const override;
	//終了
	virtual void end() override;

private:
	//終了フラグ
	bool is_end_{ false };
	World world_;
};

#endif

