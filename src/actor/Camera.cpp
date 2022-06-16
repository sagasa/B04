#include "Camera.h"
#include"IWorld.h"

//�v���C���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f,2.0f,10.0f };
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
	is_fixed_ = false;
}

//�X�V
void Camera::update(float delta_time) {
	//�v���C���[������
	player_ = world_->find_actor("Player");
	if (player_ == nullptr) {
		player_ = world_->find_actor("PlayerPaladin");
		if (player_ == nullptr) return;
	}
	//�J�����̈ʒu�����߂�
	GSvector3 position = PlayerOffset + player_->transform().position();
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_, SmoothTime, MaxSpeed, delta_time);
	//�����_�̈ʒu�����߂�
	GSvector3 at = player_->transform().position() + ReferncePointOffset;
	transform_.position(position);
	transform_.lookAt(at);
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

bool Camera::is_viewing(const BoundingSphere& sphere) {
	GSfrustum		Frustum;
	GSmatrix4		matProj;
	GSmatrix4		matView;
	/* �����ϊ��s����擾 */
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&matProj);

	/* ���f���r���[�ϊ��}�g���N�X���擾���� */
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&matView);

	/* ��������쐬���� */
	gsFrustumFromMatrices(&Frustum, &matView, &matProj);
	return gsFrustumIsSphereInside
	(
		&Frustum,
		&sphere.center,
		sphere.radius
	);
	//return gsFrustumIsPointInside(&Frustum, &sphere.center) == GS_FALSE;



}