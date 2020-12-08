#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"

//Ž€–S‚·‚é‚Ü‚Å‚ÌŽžŠÔ
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity) :
	died_timer_{0.0f} {
	world_ = world;
	name_ = "PoltergeistBullet";
	tag_ = "EnemyAttackTag";
	collider_ = BoundingSphere{0.4f };
	transform_.position(position);
	velocity_ = velocity;
}

//XV
void PoltergeistBullet::update(float delta_time) {
	//3•b‚½‚Á‚½‚çŽ€–S
	/*if (died_timer_ / 60 >= Delay_time) {
		die();
		return;
	}
	else died_timer_ += delta_time;*/
	//‰æ–ÊŠO‚Éo‚½‚çŽ€–S
	if (is_out_camera()) {
		die();
		return;
	}
	//‰ñ“]‚·‚é
	transform_.rotate(0.0f,1.0f,1.0f);
	//ˆÚ“®‚·‚é
	transform_.translate(velocity_, GStransform::Space::World);
}

//•`‰æ
void PoltergeistBullet::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(6);
	glPopMatrix();
	collider().draw();
}

//Õ“ËƒŠƒAƒNƒVƒ‡ƒ“
void PoltergeistBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}

bool PoltergeistBullet::is_out_camera() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//‰æ–Ê“à‚É‚¢‚½‚çˆÚ“®‚·‚é
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//ƒJƒƒ‰‚Ì‘OƒxƒNƒgƒ‹
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle >= 45.0f);
}