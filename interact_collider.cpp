#include "interact_collider.h"

#include <algorithm>


#include "interact_prop.h"

interact_collider::interact_collider(IWorld* world, const BoundingSphere& collider, const std::string& tag, const std::string& name, std::function<void(Actor&)> func)
{
	func_ = func;
	//ワールドの設定
	world_ = world;
	//タグ名の設定
	tag_ = tag;
	//名前の設定
	name_ = name;
	//衝突判定を初期化
	collider_ = BoundingSphere{ collider.radius };
	//座標の初期化
	transform_.position(collider.center);
}

void interact_collider::update(float delta_time)
{
	if(count_==1)
	{
		Actor* target = nullptr;
		float max = 1000000;
		std::for_each(cash_.begin(), cash_.end(), [this, &target, &max](Actor* elem)
			{
				const float dist = elem->transform().position().distance(transform_.position());
				if (dist< max)
				{
					max = dist;
					target = elem;
				}
			});
		if (target != nullptr)
			func_(*target);
	}else if(count_ == 2)
	{
		die();
	}
	++count_;
}

void interact_collider::draw() const
{
	collider().draw();
}

void interact_collider::react(Actor& other)
{
	if (interact_prop::try_interact(other, *this))
		cash_.emplace_back(&other);
}
