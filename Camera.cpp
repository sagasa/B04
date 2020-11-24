#include "Camera.h"
#include"IWorld.h"

//�v���C���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f,1.0f,10.0f };
//�J�����̒����_�̕␳�l
const GSvector3 ReferncePointOffset{ 0.0f,1.7,0.0f };
//�X���[�X�_���v�ɂ�銊�炩�ȕ��
const float SmoothTime{ 6.0f };//��ԃt���[����
const float MaxSpeed{ 1.0f };   //�ړ��X�s�[�h�̍ő�l

// �R���X�g���N�^
Camera::Camera(IWorld* world) {
    world_ = world;
    name_ = "Camera";
    tag_ = "CamaraTag";
}

//�X�V
void Camera::update(float delta_time) {
	//�v���C���[������
	player_ = world_->find_actor("Player");
	if (player_ == nullptr) return;
	//�J�����̈ʒu�����߂�
	GSvector3 position = PlayerOffset + player_->transform().position();
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_, SmoothTime, MaxSpeed, delta_time);
	//�����_�̈ʒu�����߂�
	GSvector3 at = player_->transform().position() + ReferncePointOffset;
	transform_.position(position);
	//transform_.lookAt(at);
}

// �`��
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