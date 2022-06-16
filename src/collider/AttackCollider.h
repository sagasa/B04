#ifndef ATTACK_COLLIDER_H_
#define ATTACK_COLLIDER_H_

#include"Actor.h"

//攻撃判定クラス
class AttackCollider : public Actor {
public:
	//コンストラクタ
	AttackCollider(IWorld* world, const BoundingSphere& collider,
		const std::string& tag, const std::string& name, std::string& use_name,
		float lifespan = 1.0f, float delay = 0.0f, float atk_power = 1.0f);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//寿命
	float lifespan_timer_;
	//衝突判定が有効になるまでの遅延時間
	float delay_timer_;
	//攻撃力
	float atk_power_;

	GSvector3 before_position_;
	std::string user_;
};


#endif