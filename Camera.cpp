#include "Camera.h"
#include"IWorld.h"

//�v���C���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f,1.0f,10.0f };
//�J�����̒����_�̕␳�l
const GSvector3 ReferncePointOffset{ 0.0f,1.7,0.0f };

// �R���X�g���N�^
Camera::Camera(IWorld* world) {
    world_ = world;
    name_ = "Camera";
    tag_ = "CamaraTag";
}

// �`��
void Camera::draw() const {
	//�v���C���[������
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;
	//�J�����̈ʒu�����߂�
	GSvector3 eye = PlayerOffset + player->transform().position();
	//�����_�̈ʒu�����߂�
	GSvector3 at = player->transform().position() + ReferncePointOffset;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,
		at.x, at.y, at.z,
		0.0f, 1.0f, 0.0f
	);
}