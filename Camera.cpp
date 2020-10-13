#include "Camera.h"

// コンストラクタ
Camera::Camera(IWorld* world) {
    world_ = world;
    name_ = "Camera";
    tag_ = "CamaraTag";
}

// 描画
void Camera::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 0.0f, 200.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
}