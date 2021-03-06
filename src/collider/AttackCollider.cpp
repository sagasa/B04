#include"AttackCollider.h"
#include"DamageProp.h"
#include"World.h"


//コンストラクタ
AttackCollider::AttackCollider(IWorld* world, const BoundingSphere& collider,
	const std::string& tag, const std::string& name, std::string& user_name, float lifespan, float delay, float atk_power):
	lifespan_timer_{ lifespan }, delay_timer_{ delay }, atk_power_{ atk_power }, before_position_{GSvector3::zero()}
{
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
	//無敵状態にする
	enable_collider_ = false;

	user_ = user_name;
}

//更新
void AttackCollider::update(float delta_time)
{
	//遅延時間が経過したら衝突判定を有効にする
	if (delay_timer_ <= 0.0f) {
		//衝突判定を有効にする
		enable_collider_ = true;
		//寿命が尽きたら死亡
		if (lifespan_timer_ <= 0.0f) {
			die();
		}
		//寿命の更新
		lifespan_timer_ -= delta_time;
	}
	//遅延時間の更新
	delay_timer_ -= delta_time;
	Actor* user = world_->find_actor(user_);
	if (user == nullptr) return;
	GSvector3 position = (user->transform().position() - before_position_);
	before_position_ = user->transform().position();
	//position += transform_.position();
	transform_.translate(position, GStransform::Space::World);
}

//描画
void AttackCollider::draw() const
{
	collider().draw();
}

//衝突リアクション
void AttackCollider::react(Actor& other)
{
	if(DamageProp::do_attack(other,*this,atk_power_))
		//衝突したら死亡
		die();
}