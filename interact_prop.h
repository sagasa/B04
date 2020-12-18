#pragma once

#include <typeinfo>
#include "Actor.h"

//アクタープロパティクラス
class interact_prop {
public:
	//コンストラクタ
	interact_prop() = default;

	//仮想デストラクタ
	virtual ~interact_prop() = default;

	//インタラクトを受ける
	virtual bool can_interact(const Actor& from) = 0;

	static bool try_interact(Actor& victim, const Actor& from)
	{
		try
		{
			auto& actor = dynamic_cast<interact_prop&>(victim);
			//減らす処理
			return actor.can_interact(from);
		}
		catch (std::bad_cast&)
		{
			return false;
		}
	}
};