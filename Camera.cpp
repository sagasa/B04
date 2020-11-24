#include "Camera.h"
#include"IWorld.h"

//プレイヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f,1.0f,10.0f };
//カメラの注視点の補正値
const GSvector3 ReferncePointOffset{ 0.0f,1.7,0.0f };
//スムースダンプによる滑らかな補間
const float SmoothTime{ 6.0f };//補間フレーム数
const float MaxSpeed{ 1.0f };   //移動スピードの最大値

// コンストラクタ
Camera::Camera(IWorld* world) {
    world_ = world;
    name_ = "Camera";
    tag_ = "CamaraTag";
}

//更新
void Camera::update(float delta_time) {
	//プレイヤーを検索
	player_ = world_->find_actor("Player");
	if (player_ == nullptr) return;
	//カメラの位置を求める
	GSvector3 position = PlayerOffset + player_->transform().position();
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_, SmoothTime, MaxSpeed, delta_time);
	//注視点の位置を求める
	GSvector3 at = player_->transform().position() + ReferncePointOffset;
	transform_.position(position);
	//transform_.lookAt(at);
}

// 描画
void Camera::draw() const {
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,
		at.x, at.y, 0.0f,
		0.0f, 1.0f, 0.0f
	);
}