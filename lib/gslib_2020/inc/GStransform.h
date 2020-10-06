#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <GStype.h>
#include <GSquaternion.h>
#include <GSvector3.h>
#include <GSmatrix4.h>
#include <list>

// �g�����X�t�H�[���N���X
class GStransform {
public:
	// ���W�n�̎w��
	enum class Space {
		Self,   // ���[�J�����W�n
		World   // ���[���h���W�n
	};
	// �R���X�g���N�^
	GStransform() = default;
	// �f�X�g���N�^
	~GStransform();
	// ���[���h��Ԃ� Transform �̐����擾
	GSvector3 forward() const;
	// ���[���h��Ԃ� Transform �̐���ݒ�
	void forward(const GSvector3& value);
	// ���[���h��Ԃ� Transform �̐Ԏ����擾
	GSvector3 right() const;
	// ���[���h��Ԃ� Transform �̐Ԏ���ݒ�
	void right(const GSvector3& value);
	// ���[���h��Ԃ� Transform �̗Ύ����擾
	GSvector3 up() const;
	// ���[���h��Ԃ� Transform �̗Ύ���ݒ�
	void up(const GSvector3& value);

	// �I�u�W�F�N�g�̃O���[�o���X�P�[�����擾
	GSvector3 lossyScale() const;
	// ���[���h��Ԃ� Transform �̈ʒu���擾
	GSvector3 position() const;
	// ���[���h��Ԃ� Transform �̈ʒu��ݒ�
	void position(const GSvector3& value);
	// GSquaternion �Ƃ��ĕۑ�����郏�[���h��Ԃł� Transform �̉�]���擾
	GSquaternion rotation() const;
	// GSquaternion �Ƃ��ĕۑ�����郏�[���h��Ԃł� Transform �̉�]��ݒ�
	void rotation(const GSquaternion& value);
	// �I�C���[�p�Ƃ��Ă̊p�x���擾
	GSvector3 eulerAngles() const;
	// �I�C���[�p�Ƃ��Ă̊p�x��ݒ�
	void eulerAngles(const GSvector3& value);
    void eulerAngles(float pitch, float yaw, float roll);

	// �Ώۂ� Transform ��ݒ肵�A���̕����ւƌ������܂�
	void lookAt(const GStransform& target, const GSvector3& world_up = GSvector3{ 0.0f, 1.0f, 0.0f });
	void lookAt(const GSvector3& target, const GSvector3& world_up = GSvector3{ 0.0f, 1.0f, 0.0f });
	// Z ����eulers.z �x��]�AX���� euler.x �x��]�AY����eulers.y �x��]���܂�(���Ԃ͐���������)
	void rotate(const GSvector3& eulers, Space relative_to = Space::Self);
    void rotate(float pitch, float yaw, float roll, Space relative_to = Space::Self);
    // axis ���̎���� angle �x�A��]���܂�
	void rotate(const GSvector3& axis, float angle, Space relative_to = Space::Self);
	// ���[���h���W�� point �𒆐S�Ƃ�����(axis)�� angle �x��]�����܂�
	void rotateAround(const GSvector3& point, const GSvector3& axis, float angle);

	// translation �̕����Ƌ����Ɉړ����܂�
    void translate(float x, float y, float z, Space relative_to = Space::Self);
    void translate(const GSvector3& translation, Space relative_to = Space::Self);
    void translate(float x, float y, float z, const GStransform& relative_to);
    void translate(const GSvector3& translation, const GStransform& relative_to);

	// ���[�J�����W���烏�[���h���W�֕ϊ������s��
	GSmatrix4 localToWorldMatrix() const;
	// ���[���h���W���烍�[�J�����W�֕ϊ������s��
	GSmatrix4 worldToLocalMatrix() const;

	// ���[�J����Ԃ��烏�[���h��Ԃ� position ��ϊ����܂��B(�X�P�[���E��]�E���s�ړ��j
	GSvector3 transformPoint(const GSvector3& position) const;
	// ���[�J����Ԃ��烏�[���h��Ԃ� vector ��ϊ����܂��B(�X�P�[���E��]�j
	GSvector3 transformVector(const GSvector3& vector) const;
	// ���[�J����Ԃ��烏�[���h��Ԃ� direction ��ϊ����܂�(��]�j
	GSvector3 transformDirection(const GSvector3& direction) const;

	// ���[���h��Ԃ��烍�[�J����Ԃ� position ��ϊ����܂�
	GSvector3 inverseTransformPoint(const GSvector3& position) const;
	// ���[���h��Ԃ��烍�[�J����Ԃ� vector ��ϊ����܂�
	GSvector3 inverseTransformVector(const GSvector3& vector) const;
	// ���[���h��Ԃ��烍�[�J����Ԃ� direction ��ϊ����܂�
	GSvector3 inverseTransformDirection(const GSvector3& direction) const;

	// Transform �̐e��ݒ�
	void parent(GStransform* parent);
	// Transform �̐e���擾
	GStransform* parent() const;
	// Transform �̐e��ݒ�
	void setParent(GStransform* parent, bool world_position_stays = true);

	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�ȃX�P�[�����擾
	GSvector3 localScale() const;
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�ȃX�P�[����ݒ�
	void localScale(const GSvector3& value);
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�Ȉʒu���擾
	GSvector3 localPosition() const;
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�Ȉʒu��ݒ�
	void localPosition(const GSvector3& value);
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�ȉ�]���擾
	GSquaternion localRotation() const;
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�ȉ�]��ݒ�
	void localRotation(const GSquaternion& value);
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�ȃI�C���[�p�Ƃ��Ẳ�]�l���擾
	GSvector3 localEulerAngles() const;
	// �e�� Transform �I�u�W�F�N�g���猩�����ΓI�ȃI�C���[�p�Ƃ��Ẳ�]�l��ݒ�
	void localEulerAngles(const GSvector3& value);
	// ���ׂĂ̎q�I�u�W�F�N�g��e�I�u�W�F�N�g����؂藣���܂�
	void detachChildren();
	// �e�� Transform ����؂藣��
	void detachParent();

	// ������̎擾
	void* owner() const;
	// ������̐ݒ�
	void owner(void* p);

private:
	// ���[���h���W�n�̍X�V
	void updateWorldTransform(const GStransform* parent);

private:
	// �|�W�V���� (���[���h���W�n)
	GSvector3 position_{ 0.0f, 0.0f, 0.0f };
	// ��] (���[���h���W�n)
	GSquaternion rotation_{ 0.0f, 0.0f, 0.0f, 1.0f };
	// �X�P�[�� (���[���h���W�n)
	GSvector3 scale_{ 1.0f, 1.0f, 1.0f };

	// �|�W�V���� (���[�J�����W�n)
	GSvector3 local_position_{ 0.0f, 0.0f, 0.0f };
	// ��] (���[�J�����W�n)
	GSquaternion local_rotation_{ 0.0f, 0.0f, 0.0f, 1.0f };
	// �X�P�[�� (���[�J�����W�n)
	GSvector3 local_scale_{ 1.0f, 1.0f, 1.0f };

	// �e�̃g�����X�t�H�[��
	GStransform* parent_{ nullptr };
	// �q�̃g�����X�t�H�[��
	std::list<GStransform*> children_;

	// ���̃g�����X�t�H�[���̎�����
	void* owner_{ nullptr };
};

#endif
