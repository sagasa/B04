#include "PsycokinesisBullet.h"
#include "Assets.h"
#include"IWorld.h"
#include"Field.h"

const float MaxAcceleration = 3.0f;
const GSvector3 PlayerOffset{ 0.0f,1.0f,0.0f };

PsycokinesisBullet::PsycokinesisBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float period, float power)
	:acceleration_{ GSvector3(0.0f, 0.0f, 0.0f) },
	period_{ period }
{
	world_ = world;
	name_ = "PsycokinesisBullet";
	tag_ = "EnemyAttack";	
	velocity_ = velocity;
	collider_ = BoundingSphere{ 0.3f };
	transform_.position(position);
	damage_ = power;
	set_hp(0.0f);
 }

void PsycokinesisBullet::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if(player==nullptr)
	{
		player = world_->find_actor("PlayerPaladin");
		if (player == nullptr)return;
	}	

	acceleration_ = GSvector3::zero();
	GSvector3 diff = player->transform().position() + PlayerOffset - transform_.position();
	acceleration_ += ((diff - velocity_ * period_) * 2.0f / (period_ * period_));
	if (acceleration_.magnitude() >= MaxAcceleration) {
		acceleration_ = acceleration_.normalize() * MaxAcceleration;
	}
	period_ -= (delta_time / 60.0f);
	velocity_ += acceleration_ * (delta_time / 60.0f);
	
	transform_.position(transform_.position() + velocity_ * (delta_time / 60.0f));

	if (world_->field()->collide(collider()))die();
	transform_.rotate(1.0f, 1.0f, 0.0f);
}

void PsycokinesisBullet::draw()const {
	collider().draw();
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glPushMatrix();
	glTranslated(0.0f, -0.25f, 0.0f);
	gsDrawMesh(Mesh_Book);
	glPopMatrix();
	glPopMatrix();	
}
void PsycokinesisBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		do_attack(other, *this, damage_);
		die();
	}
}

bool PsycokinesisBullet::on_hit(const Actor& attacker, float atk_power)
{
	return false;
}

