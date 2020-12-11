#ifndef ACTOR_PROP_H_
#define ACTOR_PROP_H_



//アクタープロファイルクラス
class ActorProp {
public:
	//コンストラクタ
	ActorProp() = default;

	//仮想デストラクタ
	virtual ~ActorProp() = default;

	//体力を取得
	const float hp() const {
		return hp_;
	}

	//攻撃力を取得
	const float atk_power()const {
		return atk_power_;
	}

protected:
	//体力
	float hp_{ 0.0f };
	//攻撃力
	float atk_power_{ 0.0f };

};



#endif

