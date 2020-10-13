#include <GStransform.h>

// �f�X�g���N�^
GStransform::~GStransform() {
	// �q�I�u�W�F�N�g��؂藣��
	detachChildren();
	// �e�I�u�W�F�g����؂藣��
	detachParent();
}

// ���[���h��Ԃ� GStransform�̐����擾
GSvector3 GStransform::forward() const {
	return rotation_ * GSvector3::forward();
}

// ���[���h��Ԃ� GStransform�̐���ݒ�
void GStransform::forward(const GSvector3& value) {
	rotation(GSquaternion::lookRotation(value));
}

// ���[���h��Ԃ� GStransform �̐Ԏ����擾
GSvector3 GStransform::right() const {
	return rotation_ * GSvector3::right();
}

// ���[���h��Ԃ� GStransform �̐Ԏ���ݒ�
void GStransform::right(const GSvector3& value) {
	rotation(GSquaternion::fromToRotation(GSvector3::right(), value));
}

// ���[���h��Ԃ� GStransform �̗Ύ����擾
GSvector3 GStransform::up() const {
	return rotation_ * GSvector3::up();
}

// ���[���h��Ԃ� GStransform �̗Ύ���ݒ�
void GStransform::up(const GSvector3& value) {
	rotation(GSquaternion::fromToRotation(GSvector3::up(), value));
}

// �I�u�W�F�N�g�̃O���[�o���X�P�[�����擾
GSvector3 GStransform::lossyScale() const {
	return scale_;
}

// ���[���h��Ԃ� GStransform �̈ʒu���擾
GSvector3 GStransform::position() const {
	return position_;
}

// ���[���h��Ԃ� GStransform �̈ʒu��ݒ�
void GStransform::position(const GSvector3& value) {
	if (parent_ != nullptr) {
		localPosition(parent_->inverseTransformPoint(value));
	}
	else {
		localPosition(value);
	}
}

// GSquaternion �Ƃ��ĕۑ�����郏�[���h��Ԃł� GStransform �̉�]���擾
GSquaternion GStransform::rotation() const {
	return rotation_;
}

// GSquaternion �Ƃ��ĕۑ�����郏�[���h��Ԃł� GStransform �̉�]��ݒ�
void GStransform::rotation(const GSquaternion& value) {
	if (parent_ != nullptr) {
		localRotation(GSquaternion::inverse(parent_->rotation()) * value);
	}
	else {
		localRotation(value);
	}
}

// �I�C���[�p�Ƃ��Ă̊p�x���擾
GSvector3 GStransform::eulerAngles() const {
	return rotation_.eulerAngles();
}

// �I�C���[�p�Ƃ��Ă̊p�x��ݒ�
void GStransform::eulerAngles(const GSvector3& value) {
	rotation(GSquaternion::euler(value));
}

// �I�C���[�p�Ƃ��Ă̊p�x��ݒ�
void GStransform::eulerAngles(float pitch, float yaw, float roll) {
    eulerAngles(GSvector3{ pitch, yaw, roll });
}

// �Ώۂ� GStransform ��ݒ肵�A���̕����ւƌ������܂�
void GStransform::lookAt(const GStransform& target, const GSvector3& world_up) {
	lookAt(target.position(), world_up);
}

// �Ώۂ� GStransform ��ݒ肵�A���̕����ւƌ������܂�
void GStransform::lookAt(const GSvector3& target, const GSvector3& world_up) {
	rotation(GSquaternion::lookRotation(target - position_, world_up));
}

// Z ����eulers.z �x��]�AX���� euler.x �x��]�AY����eulers.y �x��]���܂�(���Ԃ͐���������)
void GStransform::rotate(const GSvector3& eulers, Space relative_to) {
	if (relative_to == Space::Self) {
		localRotation(local_rotation_ * GSquaternion::euler(eulers.x, eulers.y, eulers.z));
	} else {
		rotation(GSquaternion::euler(eulers.x, eulers.y, eulers.z) * rotation_);
	}
}

// Z ����roll �x��]�AX���� pitch �x��]�AY����yow �x��]���܂�(���Ԃ͐���������)
void GStransform::rotate(float pitch, float yaw, float roll, Space relative_to) {
    rotate(GSvector3{ pitch, yaw, roll }, relative_to);
}

// axis ���̎���� angle �x�A��]���܂�
void GStransform::rotate(const GSvector3& axis, float angle, Space relative_to) {
	if (relative_to == Space::Self) {
		localRotation(local_rotation_ * GSquaternion::angleAxis(angle, axis));
	} else {
		rotation(GSquaternion::angleAxis(angle, axis) * rotation_);
	}
}

// ���[���h���W�� point �𒆐S�Ƃ�����(axis)�� angle �x��]�����܂�
void GStransform::rotateAround(const GSvector3& point, const GSvector3& axis, float angle) {
	GSquaternion q = GSquaternion::angleAxis(angle, axis);
	position(point + q * (position_ - point));
	rotation(q * rotation_);
}

// translation �̕����Ƌ����Ɉړ����܂�
void GStransform::translate(float x, float y, float z, Space relative_to) {
    translate(GSvector3{ x, y, z }, relative_to);
}

// translation �̕����Ƌ����Ɉړ����܂�
void GStransform::translate(const GSvector3& translation, Space relative_to) {
	if (relative_to == Space::Self) {
		position(position_ + transformDirection(translation));
	} else {
		position(position_ + translation);
	}
}

// translation �̕����Ƌ����Ɉړ����܂�
void GStransform::translate(const GSvector3& translation, const GStransform& relative_to) {
   position(position_ + relative_to.transformDirection(translation));
}

// translation �̕����Ƌ����Ɉړ����܂�
void GStransform::translate(float x, float y, float z, const GStransform& relative_to) {
    translate(GSvector3{ x, y, z }, relative_to);
}

// ���[�J�����W���烏�[���h���W�֕ϊ������s��
GSmatrix4 GStransform::localToWorldMatrix() const {
	return GSmatrix4::TRS(position_, rotation_, scale_);
}

// ���[���h���W���烍�[�J�����W�֕ϊ������s��
GSmatrix4 GStransform::worldToLocalMatrix() const {
	return localToWorldMatrix().inverse();
}

// ���[�J����Ԃ��烏�[���h��Ԃ� position ��ϊ����܂� (�X�P�[���E��]�E���s�ړ��j
GSvector3 GStransform::transformPoint(const GSvector3& position) const {
	return rotation_ * GSvector3::scale(position, scale_) + position_;
}

// ���[�J����Ԃ��烏�[���h��Ԃ� vector ��ϊ����܂� (�X�P�[���E��]�j
GSvector3 GStransform::transformVector(const GSvector3& vector) const {
	return rotation_ * GSvector3::scale(vector, scale_);
}

// ���[�J����Ԃ��烏�[���h��Ԃ� direction ��ϊ����܂�(��]�j
GSvector3 GStransform::transformDirection(const GSvector3& direction) const {
	return rotation_ * direction;
}

// ���[���h��Ԃ��烍�[�J����Ԃ� position ��ϊ����܂�
GSvector3 GStransform::inverseTransformPoint(const GSvector3& position) const {
	GSvector3 result = position;
	result -= position_;
	result = GSquaternion::inverse(rotation_) * result;
	result.x /= scale_.x;
	result.y /= scale_.y;
	result.z /= scale_.z;
	return result;
}

// ���[���h��Ԃ��烍�[�J����Ԃ� vector ��ϊ����܂�
GSvector3 GStransform::inverseTransformVector(const GSvector3& vector) const {
	GSvector3 result = vector;
	result = GSquaternion::inverse(rotation_) * result;
	result.x /= scale_.x;
	result.y /= scale_.y;
	result.z /= scale_.z;
	return result;
}

// ���[���h��Ԃ��烍�[�J����Ԃ� direction ��ϊ����܂�
GSvector3 GStransform::inverseTransformDirection(const GSvector3& direction) const {
	return GSquaternion::inverse(rotation_) * direction;
}

// �e�� GStransform ���擾
GStransform* GStransform::parent() const {
	return parent_;
}

// �e�� GStransform ��ݒ�
void GStransform::parent(GStransform* parent) {
	setParent(parent, true);
}

// �e�� GStransform ��ݒ�
void GStransform::setParent(GStransform* parent, bool world_position_stays) {
	// ���݂̐e����؂藣��
	detachParent();
	// �V�����e��ݒ�
	parent_ = parent;
	if (parent_ != nullptr) {
		// ���݂̃��[���h��Ԃ�ۂ��H
		if (world_position_stays) {
			// ���[�J����Ԃ����X�V����
			local_rotation_ = GSquaternion::inverse(parent_->rotation()) * rotation_;
			local_position_ = parent_->inverseTransformPoint(position_);
		}
		else {
			// ���݂̃��[�J����Ԃ��X�V�����Ƀ��[���h��Ԃ��X�V
			updateWorldTransform(parent_);
		}
		// �������g��e�̎q�ɓo�^
		parent_->children_.push_back(this);
	}
	else {
		// �e�����Ȃ���΁A���[�J����Ԃ̓��[���h��ԂƓ���
		local_rotation_ = rotation_;
		local_position_ = position_;
		local_scale_ = scale_;
	}
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�ȃX�P�[�����擾
GSvector3 GStransform::localScale() const {
	return local_scale_;
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�ȃX�P�[����ݒ�
void GStransform::localScale(const GSvector3& value) {
	local_scale_ = value;
	// ���[���h��Ԃ��X�V
	updateWorldTransform(parent_);
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�Ȉʒu���擾
GSvector3 GStransform::localPosition() const {
	return local_position_;
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�Ȉʒu��ݒ�
void GStransform::localPosition(const GSvector3& value) {
	local_position_ = value;
	// ���[���h��Ԃ��X�V
	updateWorldTransform(parent_);
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�ȉ�]���擾
GSquaternion GStransform::localRotation() const {
	return local_rotation_;
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�ȉ�]��ݒ�
void GStransform::localRotation(const GSquaternion& value) {
	local_rotation_ = value;
	// ���[���h��Ԃ��X�V
	updateWorldTransform(parent_);
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�ȃI�C���[�p�Ƃ��Ẳ�]�l���擾
GSvector3 GStransform::localEulerAngles() const {
	return local_rotation_.eulerAngles();
}

// �e�� GStransform �I�u�W�F�N�g���猩�����ΓI�ȃI�C���[�p�Ƃ��Ẳ�]�l��ݒ�
void GStransform::localEulerAngles(const GSvector3& value) {
	localRotation(GSquaternion::euler(value));
}

// ���ׂĂ̎q�I�u�W�F�N�g��e�I�u�W�F�N�g����؂藣���܂�
void GStransform::detachChildren() {
	// ���g�̐e���q�̐e�ɕύX����
	for (auto child : children_) {
		child->parent(parent_);
	}
}

// �e�� GStransform ����؂藣��
void GStransform::detachParent() {
	if (parent_ != nullptr) {
		// �e�̃��X�g���玩�g���폜
		parent_->children_.remove(this);
		parent_ = nullptr;
	}
}

// ������̎擾
void * GStransform::owner() const {
	return owner_;
}

// ������̐ݒ�
void GStransform::owner(void * p) {
	owner_ = p;
}

// ���[���h��Ԃ̍X�V
void GStransform::updateWorldTransform(const GStransform* parent) {
	if (parent != nullptr) {
		// ���[���h��Ԃ̍X�V
		position_ = parent->transformPoint(local_position_);
		rotation_ = parent->rotation_ * local_rotation_;
		scale_ = GSvector3::scale(parent->scale_, local_scale_);
	} else {
		// �e�����Ȃ���΃��[�J����Ԃƃ��[���h��Ԃ͓���
		position_ = local_position_;
		rotation_ = local_rotation_;
		scale_ = local_scale_;
	}
	// �q���̃��[���h��Ԃ��X�V
	for (auto child : children_) {
		child->updateWorldTransform(this);
	}
}