#include "PsycokinesisBullet.h"
#include"IWorld.h"
#include"Field.h"
#include "Line.h"

const float MaxAcceleration = 10.0f;
const GSvector3 PlayerOffset{ 0.0f,1.0f,0.0f };

PsycokinesisBullet::PsycokinesisBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float period)
	:acceleration_{ GSvector3(0.0f, 0.0f, 0.0f) },
	period_{ period }
{
	world_ = world;
	name_ = "PsycokinesisBullet";
	tag_ = "Enemy";
	transform_.position(position);
	velocity_ = velocity;
	collider_ = BoundingSphere{ 0.3f };
}

void PsycokinesisBullet::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;

	acceleration_ = GSvector3::zero();
	GSvector3 diff = player->transform().position() + PlayerOffset - transform_.position();
	acceleration_ += ((diff - velocity_ * period_) * 2.0f / (period_ * period_));
	if (acceleration_.magnitude() >= MaxAcceleration) {
		acceleration_ = acceleration_.normalize() * MaxAcceleration;
	}
	period_ -= (delta_time / 60.0f);
	if(period_<0.0f)
	{
		return;
	}
	velocity_ += acceleration_ * (delta_time / 60.0f);

	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	glBegin(GL_LINES);
	glVertex3f(line.start.x, line.start.y, line.start.z);
	glVertex3f(line.end.x, line.end.y, line.end.z);
	glEnd();
	transform_.position(transform_.position() + velocity_ * (delta_time / 60.0f));

	if (world_->field()->collide(collider()))die();
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
