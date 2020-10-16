#ifndef RUSH_ENEMY_F_H_
#define RUSH_ENEMY_F_H

#include"Actor.h"

class RushEnemyF : public Actor {
public:
	//コンストラクタ
	RushEnemyF(IWorld* world, const GSvector3& position);
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

