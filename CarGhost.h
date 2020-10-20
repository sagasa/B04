#ifndef CAR_GHOST_H_
#define CAR_GHOST_H_

#include"Actor.h"


class CarGhost : public Actor {
public:
	//コンストラクタ
	CarGhost(IWorld* world,const GSvector3& position);
	//更新
	void update(float delta_time) override;
	//描画
	void draw() const override;
	//衝突リアクション
	void react(Actor& other) override;

private:
	//タイマー
	float moving_timer_{ 0.0f };
	//動くスピード
	float speed_{ 1.0f };
};

#endif