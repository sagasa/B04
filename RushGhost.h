#ifndef RUSH_GHOST_H_
#define RUSH_GHOST_H_

#include"Actor.h"

class RushGhost : public Actor {
public:
	//コンストラクタ
	RushGhost(IWorld* world, const GSvector3& position);
	//更新
	void update(float delta_time) override;
	//描画
	void draw() const override;
	//衝突リアクション
	void react(Actor& other) override;

private:
	//タイマー
	float moving_timer_{ 0.0f};
	//動くスピード
	float speed_{ 1.0f };
};


#endif

