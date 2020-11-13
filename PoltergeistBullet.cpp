#include"PoltergeistBullet.h"


//�e�̊p�x
const float BulletAngle{ 60.0f };
//�~����
const float pie{ 3.14159265359 };

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& target) :
	target_{ target } {
	world_ = world;
	name_ = "PoltergeistBullet";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere{ 2.0f };
	transform_.position(position);
	//offset_ = transform_.position();
	//this_target_ = target_ - offset_;
	velocity_ = GSvector3{-1.0f,0.0f,0.0f};
}

//�X�V
void PoltergeistBullet::update(float delta_time) {
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

//�`��
void PoltergeistBullet::draw() const {
	collider_.draw();
}

//�Փ˃��A�N�V����
void PoltergeistBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}