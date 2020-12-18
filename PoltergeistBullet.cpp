#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"
#include"Assets.h"
#include"Field.h"

//死亡するまでの時間
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float atk_power) :
	died_timer_{0.0f} {
	//ワールドの設定
	world_ = world;
	//名前の設定
	name_ = "PoltergeistBullet";
	//タグ名の設定
	tag_ = "EnemyAttack";
	//衝突判定球の設定
	collider_ = BoundingSphere{0.4f};
	//座標の初期化
	transform_.position(position);
	//
	velocity_ = velocity;
	//ActorPropを継承しているか？
	hit_ = true;
	//攻撃力の設定
	atk_power_ = atk_power;
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
	//壁との衝突判定
	collide_field();
}

//描画
void PoltergeistBullet::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(Mesh_Book);
	glPopMatrix();
	collider().draw();
}

//衝突リアクション
void PoltergeistBullet::react(Actor& other) {
	//エネミー以外に当たったら死亡
	if (other.tag() != "EnemyTag") {
		DamageProp::do_attack(other, *this, atk_power_);
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

//壁との衝突判定
void PoltergeistBullet::collide_field() {
	//壁との衝突判定(球体との判定)
	GSvector3 center;//衝突後の球体の中心座標
	if (world_->field()->collide(collider(),&center)) {
		die();
	}
}