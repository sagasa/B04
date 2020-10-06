#include <GStype.h>
#include <GSvector2.h>
#include <GSmathf.h>
#include <cmath>
#include <limits>
#include <algorithm>

const float kEpsilon{ 1e-05f };

// GSvector2(0, 0)と同じ意味
GSvector2 GSvector2::zero() {
	return GSvector2{ 0.0f, 0.0f };
}

// GSvector2(1, 1)と同じ意味
GSvector2 GSvector2::one() {
	return GSvector2{ 1.0f, 1.0f };
}

// GSvector2(0, -1)と同じ意味
GSvector2 GSvector2::up() {
	return GSvector2{ 0.0f, -1.0f };
}

// GSvector2(0, 1)と同じ意味
GSvector2 GSvector2::down() {
	return GSvector2{ 0.0f, 1.0f };
}

// GSvector2(-1, 0)と同じ意味
GSvector2 GSvector2::left() {
	return GSvector2{ -1.0f, 0.0f };
}

// GSvector2(1, 0)と同じ意味
GSvector2 GSvector2::right() {
	return GSvector2{ 1.0f, 0.0f };
}

// ベクトルの長さ
float GSvector2::magnitude() const {
	return std::sqrt(sqrMagnitude());
}

// 正規化したベクトルを返す
GSvector2 GSvector2::normalized() const {
	GSvector2 result{ *this };
	result.normalize();
	return result;
}

// ベクトルの2乗の長さ
float GSvector2::sqrMagnitude() const {
	return (x * x) + (y * y);
}

// [0]や[1]を使用して、xやy成分にアクセス
float GSvector2::operator [] (int index) const {
	return (index == 0) ? x : y;
}

// [0]や[1]を使用して、xやy成分にアクセス
float& GSvector2::operator [] (int index) {
	return (index == 0) ? x : y;
}

// 既存のGSvector2にxとyの成分を設定
void GSvector2::set(float new_x, float new_y) {
	x = new_x;
	y = new_y;
}

// 2つのベクトルのなす角
float GSvector2::angle(const GSvector2 & from, const GSvector2 & to) {
	float denominator = std::sqrt(from.sqrMagnitude() * to.sqrMagnitude());
	if (denominator < 0.0f) return 0.0f;
	float dot = GSmathf::clamp(GSvector2::dot(from, to) / denominator, -1.0f, 1.0f);
	return std::acos(dot) * GSmathf::rad_to_deg();
}

// 大きさをmaxLengthまでに制限したvectorのコピー
GSvector2 GSvector2::clampMagnitude(const GSvector2& vector, float max_length) {
	float sqr_mag = vector.sqrMagnitude();
	if (sqr_mag > max_length * max_length) {
		return (vector / std::sqrt(sqr_mag)) * max_length;
	}
	return vector;
}

// a-b間の距離
float GSvector2::distance(const GSvector2& a, const GSvector2& b) {
	return (a - b).magnitude();
}

// 内積
float GSvector2::dot(const GSvector2& lhs, const GSvector2 & rhs) {
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

// 2つのベクトルの線形補間
GSvector2 GSvector2::lerp(const GSvector2 & a, const GSvector2 & b, float t) {
	return GSvector2::lerpUnclamped(a, b, GSmathf::clamp01(t));
}

// 2つのベクトルの線形補間
GSvector2 GSvector2::lerpUnclamped(const GSvector2 & a, const GSvector2 & b, float t) {
	return a + (b - a) * t;
}

// 2つのベクトルで各成分の最大値を使用してベクトルを作成
GSvector2 GSvector2::max(const GSvector2 & lhs, const GSvector2 & rhs) {
	return GSvector2{ std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
}

// 2つのベクトルで各成分の最小値を使用してベクトルを作成
GSvector2 GSvector2::min(const GSvector2 & lhs, const GSvector2 & rhs) {
	return GSvector2{ std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
}

// 現在位置currentからtargetに向けて移動
GSvector2 GSvector2::moveTowards(const GSvector2& current, const GSvector2& target, float max_distance_delta) {
	GSvector2 to_vector = target - current;
	float sq_dist = to_vector.sqrMagnitude();
	if (sq_dist == 0.0f || (max_distance_delta >= 0.0f && sq_dist <= max_distance_delta * max_distance_delta))
		return target;
	float dist = std::sqrt(sq_dist);
	return current + (to_vector / dist) * max_distance_delta;
}

// Returns the 2D vector perpendicular to this 2D vector.
// The result is always rotated 90 - degrees in a counter - clockwise direction
// for a 2D coordinate system where the positive Y axis goes up.
GSvector2 GSvector2::perpendicular(const GSvector2& inDirection) {
	return GSvector2{ -inDirection.y, inDirection.x };
}

// 法線を基準にしてベクトルの反射したベクトルを取得
GSvector2 GSvector2::reflect(const GSvector2 & inDirection, const GSvector2 & inNormal) {
	float factor = -2.0f * dot(inNormal, inDirection);
	return factor * inNormal + inDirection;
}

// 2つのベクトルの各成分を乗算
GSvector2 GSvector2::scale(const GSvector2 & a, const GSvector2 & b) {
	return GSvector2{ a.x * b.x, a.y * b.y };
}

// Returns the signed angle in degrees between from and to.
float GSvector2::signedAngle(const GSvector2 & from, const GSvector2 & to) {
	float unsigned_angle = angle(from, to);
	float sign = GSmathf::sign(from.x * to.y - from.y * to.x);
	return unsigned_angle * sign;
}

// 目的地に向かって時間の経過と共に徐々にベクトルを変化させる
GSvector2 GSvector2::smoothDamp(const GSvector2& current, const GSvector2& target, GSvector2& current_velocity, float smooth_time, float max_speed, float delta_time) {
	smooth_time = std::max(0.0001f, smooth_time);
	float omega = 2.0f / smooth_time;

	float a = omega * delta_time;
	float exp = 1.0f / (1.0f + a + 0.48f * a * a + 0.235f * a * a * a);

	GSvector2 change = current - target;
	GSvector2 original_to = target;

	// Clamp maximum speed
	float max_change = max_speed * smooth_time;

	float max_change_sq = max_change * max_change;
	float sq_dist = change.sqrMagnitude();
	if (sq_dist > max_change_sq) {
		float mag = std::sqrt(sq_dist);
		change = change / mag * max_change;
	}

	GSvector2 temp = (current_velocity + omega * change) * delta_time;
	current_velocity = (current_velocity - omega * temp) * exp;
	GSvector2 output = (current - change) + (change + temp) * exp;

	// Prevent overshooting
	GSvector2 orig_minus_current = original_to - current;
	GSvector2 out_minus_orig = output - original_to;

	if (orig_minus_current.sqrMagnitude() > 0.0f) {
		output = original_to;
		current_velocity = (output - original_to) / delta_time;
	}
	return output;
}
