#ifndef POLTERGEIST_BULLET_H_
#define POLTERGEIST_BULLET_H_

#include"Actor.h"

class PoltergeistBullet : public Actor {
public:

	//コンストラクタ
	PoltergeistBullet(IWorld* world, const GSvector3& position,const GSvector3& velocity);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//死亡タイマー
	float died_timer_;
};


#endif