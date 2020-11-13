#ifndef POLTERGEIST_BULLET_H_
#define POLTERGEIST_BULLET_H_

#include"Actor.h"

class PoltergeistBullet : public Actor {
public:

	//コンストラクタ
	PoltergeistBullet(IWorld* world, const GSvector3& position,const GSvector3& target);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//計算
	void parabola();

private:
	//ターゲットの位置
	GSvector3 target_;
	GSvector3 this_target_;
	//オフセット
	GSvector3 offset_;
};


#endif