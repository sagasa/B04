#include "Camera.h"
#include"IWorld.h"

//プレイヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f,1.0f,10.0f };
//カメラの注視点の補正値
const GSvector3 ReferncePointOffset{ 0.0f,1.7,0.0f };

// コンストラクタ
Camera::Camera(IWorld* world) {
    world_ = world;
    name_ = "Camera";
    tag_ = "CamaraTag";
}

// 描画
void Camera::draw() const {
	//プレイヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;
	//カメラの位置を求める
	GSvector3 eye = PlayerOffset + player->transform().position();
	//注視点の位置を求める
	GSvector3 at = player->transform().position() + ReferncePointOffset;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,
		at.x, at.y, at.z,
		0.0f, 1.0f, 0.0f
	);
}