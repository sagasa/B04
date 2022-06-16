#pragma once


#include <vector>
#include <functional>
#include"Actor.h"
#include "interact_prop.h"

//操作判定クラス
class interact_collider : public Actor {
public:
	//コンストラクタ
	interact_collider(IWorld* world, const BoundingSphere& collider,
		const std::string& tag, const std::string& name, std::function<void(Actor&)> func);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	// 毎フレーム更新
	std::vector<Actor*> cash_;
	std::function<void(Actor&)> func_;
	int count_ = 0;
};

