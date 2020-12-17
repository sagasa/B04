#include <GStype.h>
#include <GSvector3.h>
#include <GSquaternion.h>
#include <GSmathf.h>
#include <cmath>
#include <limits>
#include <algorithm>

const float kEpsilon{ 1e-05f };
const float kEpsilonNormalSqrt{ 1e-15f };

// �x�N�g���̒���
float GSvector3::magnitude() const {
	return std::sqrt(sqrMagnitude());
}

// magnitude �� 1 �Ƃ����x�N�g��
GSvector3 GSvector3::normalized() const {
	return normalize(*this);
}

// �x�N�g����2��̒���
float GSvector3::sqrMagnitude() const {
	return dot(*this, *this);
}

// [0]��[1]���g�p���āAx��y�����ɃA�N�Z�X
float GSvector3::operator [] (int index) const {
	return (index == 0) ? x : (index == 1) ? y : z;
}

// [0]��[1]���g�p���āAx��y�����ɃA�N�Z�X
float& GSvector3::operator [] (int index) {
	return (index == 0) ? x : (index == 1) ? y : z;
}

// 2�̃x�N�g���̊e��������Z���܂�
void GSvector3::scale(const GSvector3& vector) {
	*this = scale(*this, vector);
}

// ������ GSvector3 �� x�Ay�Az �̐�����ݒ肵�܂�
void GSvector3::set(float new_x, float new_y, float new_z) {
	x = new_x; y = new_y; z = new_z;
}

// 2�_�ԁifrom �� to�j�̊p�x��Ԃ��܂�
float GSvector3::angle(const GSvector3& from, const GSvector3& to) {
	// sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
	float denominator = std::sqrt(from.sqrMagnitude() * to.sqrMagnitude());
	if (denominator < kEpsilonNormalSqrt)
		return 0.0f;
	float cos = GSmathf::clamp(dot(from, to) / denominator, -1.0f, 1.0f);
	return GSmathf::rad_to_deg() * std::acos(cos);
}

// �傫����max_length�܂łɐ������� vector �̃R�s�[��Ԃ��܂�
GSvector3 GSvector3::clampMagnitude(const GSvector3& vector, float max_length) {
	if (vector.sqrMagnitude() > max_length * max_length)
		return vector.normalized() * max_length;
	return vector;
}

// 2�̃x�N�g���̊O��
GSvector3 GSvector3::cross(const GSvector3& lhs, const GSvector3& rhs) {
	return GSvector3{ lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
}

// a �� b �̊Ԃ̋�����Ԃ��܂�
float GSvector3::distance(const GSvector3& a, const GSvector3& b) {
	return (a - b).magnitude();
}

// 2�̃x�N�g���̓���
float GSvector3::dot(const GSvector3& lhs, const GSvector3& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// ������ɂ��� 2 �̃x�N�g���Ԃ��Ԃ��܂�
GSvector3 GSvector3::lerp(const GSvector3& a, const GSvector3& b, float t) {
	return lerpUnclamped(a, b, GSmathf::clamp01(t));
}

// ������ɂ��� 2 �̃x�N�g���Ԃ��Ԃ��܂�
GSvector3 GSvector3::lerpUnclamped(const GSvector3& a, const GSvector3& b, float t) {
	return a + (b - a) * t;
}

// 2�̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬���܂�
GSvector3 GSvector3::max(const GSvector3& lhs, const GSvector3& rhs) {
	return GSvector3{ std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z) };
}

// 2�̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬���܂�
GSvector3 GSvector3::min(const GSvector3& lhs, const GSvector3& rhs) {
	return GSvector3{ std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z) };
}

// ���K������
GSvector3 GSvector3::normalize(const GSvector3& value) {
	float mag = value.magnitude();
	if (mag > kEpsilon)
		return value / mag;
	return zero();
}

// �x�N�g�������K�����ꑼ�̃x�N�g���ƒ�������悤�ɂ��܂�
void GSvector3::orthoNormalize(GSvector3& normal, GSvector3& tangent, GSvector3& binormal) {
	normal.normalize();
	if (normal.sqrMagnitude() < (kEpsilon * kEpsilon)) {
		normal = GSvector3{ 1.0f, 0.0f, 0.0f };
	}
	GSvector3 bn = cross(normal, tangent);
	if (binormal.sqrMagnitude() < (kEpsilon * kEpsilon)) {
		tangent = cross(GSvector3{ 0.0f, 0.0f, 1.0f }, normal);
	}
	if (tangent.sqrMagnitude() < (kEpsilon * kEpsilon)) {
		tangent = cross(GSvector3{ 1.0f, 0.0f, 0.0f }, normal);
	}
	bn = cross(normal, tangent);
	bn.normalize();
	tangent = cross(bn, normal);
	binormal = (GSvector3::dot(bn, binormal) < 0.0f) ? -bn : bn;
}

// 2�̃x�N�g���̊e��������Z���܂�
GSvector3 GSvector3::scale(const GSvector3& a, const GSvector3& b) {
	return GSvector3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

// �x�N�g����ʂ̃x�N�g���ɓ��e���܂�
GSvector3 GSvector3::project(const GSvector3& vector, const GSvector3& on_normal) {
	float sqr_mag = on_normal.sqrMagnitude();
	if (sqr_mag < kEpsilon)
		return zero();
	return on_normal * dot(vector, on_normal) / sqr_mag;
}

// ���ʂɐ����Ȗ@���x�N�g���ɂ���Ē�`����镽�ʏ�Ƀx�N�g�����ˉe���܂�
GSvector3 GSvector3::projectOnPlane(const GSvector3& vector, const GSvector3& planeNormal) {
	return vector - project(vector, planeNormal);
}

// �@���Œ�`���ꂽ���ʂŃx�N�g���𔽎˂��܂�
GSvector3 GSvector3::reflect(const GSvector3& inDirection, const GSvector3& inNormal) {
	return -2.0f * dot(inNormal, inDirection) * inNormal + inDirection;
}

// ���݂̈ʒu current ���� target �Ɍ����ăx�N�g������]���܂��B
GSvector3 GSvector3::rotateTowards(const GSvector3& current, const GSvector3& target, float max_radian_delta, float max_magnitude_delta) {
	// �p�x�ɕϊ�
	float max_degree_delta = max_radian_delta * GSmathf::rad_to_deg();
	// �x�N�g���̒��������߂�
	float current_mag = current.magnitude();
	float target_mag = target.magnitude();
	// �x�N�g���̐��K��
	GSvector3 current_normal = current / current_mag;
	GSvector3 target_normal = target / target_mag;
	// 2�x�N�g���Ԃ̊p�x�����߂�
	float angle = GSvector3::angle(current_normal, target_normal);
	// ��]�̎����v�Z
	GSvector3 axis = GSvector3::cross(current_normal, target_normal);
	// ��]�̎����ł��Ȃ������ꍇ�̕␳
	if (axis.sqrMagnitude() < (kEpsilon * kEpsilon)) {
		axis = GSvector3::cross(GSvector3{ 0.0f, 0.0f, 1.0f }, current);
		if (axis.sqrMagnitude() < (kEpsilon * kEpsilon)) {
			axis = GSvector3::cross(GSvector3{ 1.0f, 0.0f, 0.0f }, current);
		}
	}
	// �p�x�̕�Ԃ�����
	float angle_towards = GSmathf::moveTowards(0.0f, angle, max_degree_delta);
	// �����̕�Ԃ�����
	float magnitude_toward = GSmathf::moveTowards(current_mag, target_mag, max_magnitude_delta);
	return GSquaternion::angleAxis(angle_towards, axis) * current_normal * magnitude_toward;
}

// ���݂̈ʒu current ���� target �Ɍ����Ĉړ����܂�
GSvector3 GSvector3::moveTowards(const GSvector3& current, const GSvector3& target, float max_distance_delta) {
	GSvector3 to_vector = target - current;
	float dist = to_vector.magnitude();
	if (dist <= max_distance_delta || dist < kEpsilon)
		return target;
	return current + to_vector / dist * max_distance_delta;
}

// 2�_�ԁifrom �� to�j�̕����t���p�x��Ԃ��܂�
float GSvector3::signed_angle(const GSvector3& from, const GSvector3& to, const GSvector3& axis) {
	float unsigned_angle = angle(from, to);
	float sign = GSmathf::sign(dot(axis, cross(from, to)));
	return unsigned_angle * sign;
}

// 2�_�ԁifrom �� to�j�̕����t���p�x��Ԃ��܂�
float GSvector3::signedAngle(const GSvector3& from, const GSvector3& to, const GSvector3& axis) {
	float unsigned_angle = angle(from, to);
	float sign = GSmathf::sign(dot(axis, cross(from, to)));
	return unsigned_angle * sign;
}

// ����� 2 �̃x�N�g���Ԃ��Ԃ��܂�
GSvector3 GSvector3::slerp(const GSvector3& a, const GSvector3& b, float t) {
	return slerpUnclamped(a, b, GSmathf::clamp01(t));
}

// ����� 2 �̃x�N�g���Ԃ��Ԃ��܂�
GSvector3 GSvector3::slerpUnclamped(const GSvector3& a, const GSvector3& b, float t) {
	// ��]�̎����v�Z
	GSvector3 axis = GSvector3::cross(a, b);
	// ��]�̎����ł��Ȃ������ꍇ�̕␳
	if (axis.sqrMagnitude() < (kEpsilon * kEpsilon)) {
		axis = GSvector3::cross(GSvector3{ 0.0f, 0.0f, 1.0f }, a);
		if (axis.sqrMagnitude() < (kEpsilon * kEpsilon)) {
			axis = GSvector3::cross(GSvector3{ 1.0f, 0.0f, 0.0f }, a);
		}
	}
	// ��]�̕�Ԃ��s��
	return GSquaternion::angleAxis(angle(a, b) * t, axis) * a.normalized() * GSmathf::lerpUnclamped(a.magnitude(), b.magnitude(), t);
}

// �ړI�n�Ɍ������Ď��Ԃ̌o�߂ƂƂ��ɏ��X�Ƀx�N�g����ω������܂�
GSvector3 GSvector3::smoothDamp(const GSvector3& current, const GSvector3& target, GSvector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	smoothTime = std::max(0.0001f, smoothTime);
	float omega = 2.0f / smoothTime;

	float x = omega * deltaTime;
	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	GSvector3 change = current - target;
	GSvector3 originalTo = target;

	float maxChange = maxSpeed * smoothTime;
	change = clampMagnitude(change, maxChange);

	GSvector3 temp = (currentVelocity + omega * change) * deltaTime;
	currentVelocity = (currentVelocity - omega * temp) * exp;
	GSvector3 output = (current - change) + (change + temp) * exp;

	if (GSvector3::dot(originalTo - current, output - originalTo) > 0.0f) {
		output = originalTo;
		currentVelocity = (output - originalTo) / deltaTime;
	}
	return output;
}

// GSvector3(0, 0, 0) �Ɠ����Ӗ�
GSvector3 GSvector3::zero() {
	return GSvector3{ 0.0, 0.0f, 0.0f };
}

// GSvector3(1, 1, 1) �Ɠ����Ӗ�
GSvector3 GSvector3::one() {
	return GSvector3{ 1.0, 1.0f, 1.0f };
}

// GSvector3(0, 1, 0) �Ɠ����Ӗ�
GSvector3 GSvector3::up() {
	return GSvector3{ 0.0, 1.0f, 0.0f };
}

// GSvector3(0, -1, 0) �Ɠ����Ӗ�
GSvector3 GSvector3::down() {
	return GSvector3{ 0.0, 1.0f, 0.0f };
}

// GSvector3(-1, 0, 0) �Ɠ����Ӗ�
GSvector3 GSvector3::left() {
	return GSvector3{ 1.0, 0.0f, 0.0f };
}

// GSvector3(1, 0, 0) �Ɠ����Ӗ�
GSvector3 GSvector3::right() {
	return GSvector3{ -1.0, 0.0f, 0.0f };
}

// GSvector3(0, 0, 1) �Ɠ����Ӗ�
GSvector3 GSvector3::forward() {
	return GSvector3{ 0.0, 0.0f, 1.0f };
}

// GSvector3(0, 0, -1) �Ɠ����Ӗ�
GSvector3 GSvector3::back() {
	return GSvector3{ 0.0, 0.0f, -1.0f };
}

// ���̖�����
GSvector3 GSvector3::positiveInfinity() {
	return GSvector3{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
}

// ���̖�����
GSvector3 GSvector3::negativeInfinity() {
	return -positiveInfinity();
}
