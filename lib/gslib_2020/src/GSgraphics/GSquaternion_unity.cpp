#include <GStype.h>
#include <GSquaternion.h>
#include <GSmathf.h>
#include <GSvector3.h>
#include <algorithm>

const float kEpsilon{ 1e-05f };

inline bool is_equal_using_dot(float dot);
inline GSvector3 internal_make_positive(GSvector3 euler);

// �P�ʃN�H�[�^�j�I����Ԃ�
GSquaternion GSquaternion::identity() {
    return GSquaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
}

// ��]���I�C���[�p�̒l�ŕԂ��܂�
GSvector3 GSquaternion::eulerAngles() const {
	GSvector3 ax = *this * GSvector3{ 1.0f, 0.0f, 0.0f };
	GSvector3 ay = *this * GSvector3{ 0.0f, 1.0f, 0.0f };
	GSvector3 az = *this * GSvector3{ 0.0f, 0.0f, 1.0f };
	GSvector3 result = GSvector3(0.0f, 0.0f, 0.0f);
	if (az.y < 1.0f) {
		if (az.y > -1.0f) {
			result.x = std::asin(-az.y);
			result.y = std::atan2(az.x, az.z);
			result.z = std::atan2(ax.y, ay.y);
		}
		else {
			// Not a unique solution.
			result.x = GSmathf::pi() / 2.0f;
			result.y = -std::atan2(-ay.x, ax.x);
			result.z = 0.0f;
		}
	}
	else {
		// Not a unique solution.
		result.x = -GSmathf::pi() / 2.0f;
		result.y = std::atan2(-ay.x, ax.x);
		result.z = 0.0f;
	}
	return internal_make_positive(result * GSmathf::rad_to_deg());
}

// ��]���I�C���[�p�̒l�Őݒ肵�܂�
void GSquaternion::eulerAngles(const GSvector3& value) {
	*this = euler(value);
}

// ���K��
GSquaternion GSquaternion::normalized() const {
	return normalize(*this);
}

// [0] ��[1] ���g�p���� x �� y �����ɃA�N�Z�X���܂�
float GSquaternion::operator[](int index) const {
	return (index == 0) ? x : (index == 2) ? y : (index == 3) ? z : w;
}
float& GSquaternion::operator[](int index) {
	return (index == 0) ? x : (index == 2) ? y : (index == 3) ? z : w;
}

// ������ GSquaternion �� x�Ay�Az�Aw �̐�����ݒ肵�܂�
void GSquaternion::set(float newX, float newY, float newZ, float newW) {
	x = newX; y = newY;  z = newZ; w = newW;
}

// fromDirection ���� toDirection �ւ̉�]���쐬���܂�
void GSquaternion::setFromToRotation(const GSvector3& fromDirection, const GSvector3& toDirection) {
	*this = fromToRotation(fromDirection, toDirection);
}

// �w�肳�ꂽ forward �� upwards �����ɉ�]���܂�
void GSquaternion::setLookRotation(const GSvector3& view, const GSvector3& up) {
	*this = lookRotation(view, up);
}

// ��]�����W�ɑ΂���p�x�̒l (AngleAxis) �ɕϊ����܂��B
void GSquaternion::toAngleAxis(float& angle, GSvector3& axis) {
	float a = std::acos(w);
	float s = std::sin(a);
	angle = a * 2.0f * GSmathf::rad_to_deg();
	if (s > kEpsilon) {
		axis = GSvector3{ x, y, z } *(1.0f / s);
	}
	else {
		axis = GSvector3{ 1.0f, 0.0f, 0.0f };
		angle = 0.0f;
	}
}

// 2�̉�]a��b�Ԃ̊p�x��Ԃ��܂��B
float GSquaternion::angle(const GSquaternion& a, const GSquaternion& b) {
	float dot = GSquaternion::dot(a, b);
	return is_equal_using_dot(dot) ? 0.0f : std::acos(std::min(std::abs(dot), 1.0f)) * 2.0f * GSmathf::rad_to_deg();
}

// axis �̎���� angle �x��]�����]���쐬���܂��B
GSquaternion GSquaternion::angleAxis(float angle, const GSvector3& axis) {
	float rad = angle * 0.5f * GSmathf::deg_to_rad();
	float s = std::sin(rad) / axis.magnitude();
	return GSquaternion{ axis.x * s, axis.y * s, axis.z * s, std::cos(rad) };
}

// 2 �̉�]�̓��ς�Ԃ��܂�
float GSquaternion::dot(const GSquaternion& a, const GSquaternion& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// z���𒆐S��z�x�Ax���𒆐S��x�x�Ay���𒆐S��y�x��]�����]��Ԃ��܂��B
GSquaternion GSquaternion::euler(float x, float y, float z) {
	return euler(GSvector3{ x, y, z });
}

// z���𒆐S��z�x�Ax���𒆐S��x�x�Ay���𒆐S��y�x��]�����]��Ԃ��܂��B
GSquaternion GSquaternion::euler(const GSvector3& euler) {
	GSvector3 v = euler * GSmathf::deg_to_rad();
	float c1 = std::cos(v.x * 0.5f);
	float c2 = std::cos(v.y * 0.5f);
	float c3 = std::cos(v.z * 0.5f);
	float s1 = std::sin(v.x * 0.5f);
	float s2 = std::sin(v.y * 0.5f);
	float s3 = std::sin(v.z * 0.5f);
	float x = s1 * c2 * c3 + c1 * s2 * s3;
	float y = c1 * s2 * c3 - s1 * c2 * s3;
	float z = c1 * c2 * s3 - s1 * s2 * c3;
	float w = c1 * c2 * c3 + s1 * s2 * s3;
	return GSquaternion{ x, y, z, w };
}

// fromDirection ���� toDirection �ւ̉�]���쐬���܂��B
GSquaternion GSquaternion::fromToRotation(const GSvector3& fromDirection, const GSvector3& toDirection) {
	// 2�x�N�g���Ԃ̊p�x�����߂�
	float angle = GSvector3::angle(fromDirection, toDirection);
	if (angle < kEpsilon) {
		return GSquaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
	}
	// ��]�̎����v�Z
	GSvector3 axis = GSvector3::cross(fromDirection, toDirection);
	// ��]�̎����ł��Ȃ������ꍇ�̕␳
	if (axis.sqr_magnitude() < (kEpsilon * kEpsilon)) {
		axis = GSvector3::cross(GSvector3{ 0.0f, 0.0f, 1.0f }, fromDirection);
		if (axis.sqr_magnitude() < (kEpsilon * kEpsilon)) {
			axis = GSvector3::cross(GSvector3{ 1.0f, 0.0f, 0.0f }, fromDirection);
		}
	}
	return angleAxis(angle, axis);
}

// �t�N�H�[�^�j�I����Ԃ��܂�
GSquaternion GSquaternion::inverse(const GSquaternion& rotation) {
	return GSquaternion{ -rotation.x, -rotation.y, -rotation.z, rotation.w };
}

// �w�肳�ꂽ forward �� upwards �����ɉ�]���܂�
GSquaternion GSquaternion::lookRotation(const GSvector3& view, const GSvector3& up) {
	GSvector3 forward = GSvector3::normalize(view);
	if (forward.sqr_magnitude() < (kEpsilon * kEpsilon)) {
		forward = GSvector3{ 0.0f, 0.0f, 1.0f };
	}
	GSvector3 left = GSvector3::cross(up, forward);
	if (left.sqr_magnitude() < (kEpsilon * kEpsilon)) {
		left = GSvector3{ 1.0f, 0.0f, 0.0f };
	}
	GSvector3 upwards = GSvector3::cross(forward, left);
	upwards.normalize();
	left = GSvector3::cross(upwards, forward);
	GSquaternion result;
	float radicand = left.x + upwards.y + forward.z;
	if (radicand > 0.0f) {
		result.w = std::sqrt(1.0f + radicand) * 0.5f;
		float recip = 1.0f / (4.0f * result.w);
		result.x = (upwards.z - forward.y) * recip;
		result.y = (forward.x - left.z) * recip;
		result.z = (left.y - upwards.x) * recip;
	}
	else if (left.x >= upwards.y && left.x >= forward.z) {
		result.x = std::sqrt(1.0f + left.x - upwards.y - forward.z) * 0.5f;
		float recip = 1.0f / (4.0f * result.x);
		result.w = (upwards.z - forward.y) * recip;
		result.z = (forward.x + left.z) * recip;
		result.y = (left.y + upwards.x) * recip;
	}
	else if (upwards.y > forward.z) {
		result.y = std::sqrt(1.0f - left.x + upwards.y - forward.z) * 0.5f;
		float recip = 1.0f / (4.0f * result.y);
		result.z = (upwards.z + forward.y) * recip;
		result.w = (forward.x - left.z) * recip;
		result.x = (left.y + upwards.x) * recip;
	}
	else {
		result.z = std::sqrt(1.0f - left.x - upwards.y + forward.z) * 0.5f;
		float recip = 1.0f / (4.0f * result.z);
		result.y = (upwards.z + forward.y) * recip;
		result.x = (forward.x + left.z) * recip;
		result.w = (left.y - upwards.x) * recip;
	}
	return result;
}

// ���K�������N�H�[�^�j�I����Ԃ��܂�
GSquaternion GSquaternion::normalize(const GSquaternion& q) {
	float mag = std::sqrt(dot(q, q));
	if (mag < kEpsilon) {
		return GSquaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
	}
	return GSquaternion{ q.x / mag, q.y / mag, q.z / mag, q.w / mag };
}

// from ���� to �ւ̉�]�𓾂܂�
GSquaternion GSquaternion::rotateTowards(const GSquaternion& from, const GSquaternion& to, float maxDegreesDelta) {
	float angle = GSquaternion::angle(from, to);
	if (angle == 0.0f) return to;
	return slerpUnclamped(from, to, std::min(1.0f, maxDegreesDelta / angle));
}

// a��b�̊Ԃ�t�Ő��`�ɕ�Ԃ��܂��B�p�����[�^�[t�́A[0, 1] �͈̔͂ł��B
GSquaternion GSquaternion::lerp(const GSquaternion& a, const GSquaternion& b, float t) {
	return lerpUnclamped(a, b, GSmathf::clamp01(t));
}

// a��b�̊Ԃ�t�ŋ���ɕ�Ԃ��܂��B�p�����[�^�[t�́A�����Ă��܂���B
GSquaternion GSquaternion::lerpUnclamped(const GSquaternion& a, const GSquaternion& b, float t) {
	return (a * (1.0f - t) + b * t).normalized();
}

// a��b�̊Ԃ�t�ŋ���ɕ�Ԃ��܂��B�p�����[�^�[t�́A[0, 1] �͈̔͂ł��B
GSquaternion GSquaternion::slerp(const GSquaternion& a, const GSquaternion& b, float t) {
	return slerpUnclamped(a, b, GSmathf::clamp01(t));
}

// a��b�̊Ԃ�t�ŋ���ɕ�Ԃ��܂��B�p�����[�^�[t�́A�����Ă��܂���B
GSquaternion GSquaternion::slerpUnclamped(const GSquaternion& a, const GSquaternion& b, float t) {
	float cos = dot(a, b);
	float sign = 1.0f;
	if (cos < 0.0f) {
		cos = -cos;
		sign = -1.0f;
	}
	float t0 = 1.0f - t;
	float t1 = t;
	if (cos < 0.999f) {
		float theta = std::acos(cos);
		t0 = std::sin(theta * t0) / std::sin(theta);
		t1 = std::sin(theta * t1) / std::sin(theta);
	}
	return ((a * t0) + (b * sign * t1)).normalized();
}

// Rotates the point /point/ with /rotation/.
GSvector3 operator * (const GSquaternion& rotation, const GSvector3& point) {
	float x = rotation.x * 2.0f;
	float y = rotation.y * 2.0f;
	float z = rotation.z * 2.0f;
	float xx = rotation.x * x;
	float yy = rotation.y * y;
	float zz = rotation.z * z;
	float xy = rotation.x * y;
	float xz = rotation.x * z;
	float yz = rotation.y * z;
	float wx = rotation.w * x;
	float wy = rotation.w * y;
	float wz = rotation.w * z;
	GSvector3 res;
	res.x = (1.0f - (yy + zz)) * point.x + (xy - wz) * point.y + (xz + wy) * point.z;
	res.y = (xy + wz) * point.x + (1.0f - (xx + zz)) * point.y + (yz - wx) * point.z;
	res.z = (xz - wy) * point.x + (yz + wx) * point.y + (1.0f - (xx + yy)) * point.z;
	return res;
}

// 0�`360�̃v���X�̊p�x�ɕϊ�
inline GSvector3 internal_make_positive(GSvector3 euler) {
	float negative_flip = -0.0001f * GSmathf::rad_to_deg();
	float positive_flip = 360.0f + negative_flip;

	if (euler.x < negative_flip)
		euler.x += 360.0f;
	else if (euler.x > positive_flip)
		euler.x -= 360.0f;

	if (euler.y < negative_flip)
		euler.y += 360.0f;
	else if (euler.y > positive_flip)
		euler.y -= 360.0f;

	if (euler.z < negative_flip)
		euler.z += 360.0f;
	else if (euler.z > positive_flip)
		euler.z -= 360.0f;

	return euler;
}

// ���ς��r����ۂɌ덷���l��
inline bool is_equal_using_dot(float dot) {
	// Returns false in the presence of NaN values.
	return dot > 1.0f - kEpsilon;
}
