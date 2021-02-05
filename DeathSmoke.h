#ifndef DEATH_SMOKE_H_
#define DEATH_SMOKE_H_

#include"Actor.h"
#include"DamageProp.h"

class DeathSmoke : public Actor {
public:

	//コンストラクタ
	DeathSmoke(IWorld* world, const GSvector3& position);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//画面外か？
	bool is_out_camera() const;
	//壁との衝突判定
	void collide_field();
	
};


#endif