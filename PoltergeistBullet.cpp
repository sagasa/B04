#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"

//死亡するまでの時間
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity) :
	died_timer_{0.0f} {
	world_ = world;
	name_ = "PoltergeistBullet";
	tag_ = "EnemyAttackTag";
	collider_ = BoundingSphere{0.25f };
	transform_.position(position);
	velocity_ = velocity;
}

//更新
void PoltergeistBullet::update(float delta_time) {
	//3秒たったら死亡
	/*if (died_timer_ / 60 >= Delay_time) {
		die();
		return;
	}
	else died_timer_ += delta_time;*/
	//画面外に出たら死亡
	if (is_out_camera()) {
		die();
		return;
	}
	//回転する
	transform_.rotate(0.0f,1.0f,1.0f);
	//移動する
	transform_.translate(velocity_, GStransform::Space::World);
}

//描画
void PoltergeistBullet::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(6);
	glPopMatrix();
	//collider().draw();
}

//衝突リアクション
void PoltergeistBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}

bool PoltergeistBullet::is_out_camera() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//画面内にいたら移動する
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//カメラの前ベクトル
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle >= 45.0f);
}