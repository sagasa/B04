#ifndef ENEMY_TEST_SCENE_H_
#define ENEMY_TEST_SCENE_H_

#include"IScene.h"
#include"World.h"


class EnemyTestScene : public IScene {
public:
	//開始
	void start(int number) override;
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end()const override;
	//次のシーンを返す
	virtual std::string next()const override;
	//現在のステージ番号を返す
	virtual int stage_number() const override;
	//終了
	virtual void end()override;

private:
	//終了フラグ
	bool is_end_{ false };
	//ワールド
	World world_;

};
#endif