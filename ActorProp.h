#ifndef ACTOR_PROP_H_
#define ACTOR_PROP_H_
#include <typeinfo>


//アクタープロパティクラス
class ActorProp {

public:
	//コンストラクタ
	ActorProp() = default;

	//仮想デストラクタ
	virtual ~ActorProp() = default;

	//体力を設定
	void set_hp(float hp) {
		hp_ = hp;
	}

	//体力を取得
	float hp() const {
		return hp_;
	}

	//攻撃力を設定
	void set_atk_power(float atk) {
		atk_power_ = atk;
	}

	//攻撃力を取得
    float atk_power()const {
		return atk_power_;
	}

	//攻撃を受けた
	virtual void on_hit(const Actor& attacker,float atk_power)
	{
		hp_ -= atk_power;
	}

	static bool do_attack(Actor& victim,const Actor& attacker, float value = 1)
	{
		try
		{
			auto actor = dynamic_cast<ActorProp&>(victim);
			//減らす処理
			actor.hp_ -= value;
			actor.on_hit(attacker,value);
			return true;
		}catch (std::bad_cast&)
		{
			return false;
		}
	}

protected:
	//体力
	float hp_{ 0.0f };
	//攻撃力
	float atk_power_{ 0.0f };

};
#endif

