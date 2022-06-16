#ifndef POLTERGEIST_BULLET_H_
#define POLTERGEIST_BULLET_H_

#include"Actor.h"
#include"DamageProp.h"
#include "AnimatedMesh.h"

class PoltergeistBullet : public Actor {
public:

	//コンストラクタ
	PoltergeistBullet(IWorld* world, const GSvector3& position,const GSvector3& velocity, float atk_power = 1.0f);
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

private:
	//死亡タイマー
	float died_timer_;
	//攻撃力
	float atk_power_;

	GSvector3 camera_pos{ 0,0,0 };

	

};
#endif