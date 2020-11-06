#include "PsycokinesisBullet.h"
#include"IWorld.h"
#include"Field.h"

const float MaxAcceleration = 100.0f;

PsycokinesisBullet::PsycokinesisBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity)
	:acceleration_{ GSvector3(0.0f, 0.0f, 0.0f) }, period_{ 3.0f }
{
	world_ = world;
	name_ = "PsycokinesisBullet";
	tag_ = "Enemy";
	transform_.position(position);
	velocity_ = velocity;
	collider_ = BoundingSphere{ 3 };
}

void PsycokinesisBullet::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;

	acceleration_ = GSvector3::zero();
	GSvector3 diff = player->transform().position() - transform_.position();
	acceleration_ += ((diff - velocity_ * period_) * 2.0f / (period_ * period_));
	if (acceleration_.magnitude() >= MaxAcceleration) {
		acceleration_ = acceleration_.normalize() * 100.0f;
	}
	period_ -= (delta_time / 60.0f);

	velocity_ += acceleration_ * (delta_time / 60.0f);
	transform_.position(transform_.position() + velocity_ * (delta_time / 60.0f));
}

void PsycokinesisBullet::draw()const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	collider_.draw();
	glPopMatrix();

}
void PsycokinesisBullet::react(Actor& other) {
	if (other.name() == "Player") {
		die();
	}
}
