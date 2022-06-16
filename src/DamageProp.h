#ifndef ACTOR_PROP_H_
#define ACTOR_PROP_H_
#include <typeinfo>


//アクタープロパティクラス
class DamageProp {

public:
	//コンストラクタ
	DamageProp() = default;

	//仮想デストラクタ
	virtual ~DamageProp() = default;

	//体力を設定
	void set_hp(float hp) {
		hp_ = hp;
	}

	//体力を取得
	float hp() const {
		return hp_;
	}


	//攻撃を受けた
	virtual bool on_hit(const Actor& attacker, float atk_power) = 0;

	static bool do_attack(Actor& victim,const Actor& attacker, float value = 1)
	{
		try
		{
			auto& actor = dynamic_cast<DamageProp&>(victim);
			//減らす処理
			return actor.on_hit(attacker, value);
		}catch (std::bad_cast&)
		{
			return false;
		}
	}

protected:
	//体力
	float hp_{ 0.0f };
};
#endif

