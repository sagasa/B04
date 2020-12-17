#include "ChangeItem.h"

ChangeItem::ChangeItem(IWorld* world, const GSvector3& position)
{
	world_ = world;
	name_ = "ChangeItem";
	tag_ = "EnemyAttackTag";
	transform_.position(position);
	collider_ = BoundingSphere{ 0.3f };
}


void ChangeItem::update(float delta_time)
{
}

void ChangeItem::draw() const
{
}
