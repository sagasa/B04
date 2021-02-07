#ifndef BOSS_TEST_SCENE
#define BOSS_TEST_SCENE

#include "IScene.h"
#include"World.h"
class BossTestScene :
	public IScene
{
	//開始
	virtual void start(int numer) override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end() const override;
	//次の市0ン名を返す
	virtual std::string next() const override;
	//現在のステージ番号を返す
	virtual int stage_number() const override;
	//終了
	virtual void end() override;

private:
	World world_;
	bool load_{ false };
};

#endif