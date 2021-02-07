#include "DeathSmoke.h"
#include "ParticleManager.h"
#include "IWorld.h"
#include "ParticleManager.h"

//当たり判定の位置の高さ
const float Height{ 0.75f };
//当たり判定の半径
const float Radius{ 1.0f };
//エフェクト版半径
const float Efect_radius{ 1.2f };
//センター
const GSvector3 Center{ 0,Height,0 };
//足元のオフセット
const float FootOffset{ 0.1f };
//エフェクトインターバル
const float Interval{ 240.0f };



DeathSmoke::DeathSmoke(IWorld* world, const GSvector3& position) 
{
	//ワールド
	world_ = world;
	//名前の設定
	name_ = "DeathSmoke";
	//タグ名の設定
	tag_ = "FieldDamage";
	//体力無し
	//衝突判定球の設定
	collider_ = BoundingSphere{ Radius,Center };
	transform_.position(position);
	//mesh_.transform(transform_.localToWorldMatrix());
}
void DeathSmoke::update(float delta_time)
{
	GSvector3 position{ transform_.position() };
	GSvector3 center_position{position};//真ん中のエフェクトの位置
	//真ん中に調整
	center_position.y= position.y  + Height;
	GSvector3 top_position{ center_position };//上
	top_position.y +=Radius/2;
	GSvector3 under_position{ center_position };//下
	under_position.y -= Radius/2;
	GSvector3 right_position{ center_position };//右
	right_position.x += Radius / 2;
	GSvector3 left_position{ center_position };//左
	left_position.x -= Radius / 2;

	world_->particle_manager()->death_smoke2(center_position);
	/*world_->particle_manager()->death_smoke(top_position);
	world_->particle_manager()->death_smoke(under_position);
	world_->particle_manager()->death_smoke(right_position);
	world_->particle_manager()->death_smoke(left_position);*/

}
void DeathSmoke::draw() const
{
	collider().draw();
}
void DeathSmoke::react(Actor& other)
{
	if (other.tag() == "PlayerTag")
	{
		DamageProp::do_attack(other, *this, 100);
	}
}