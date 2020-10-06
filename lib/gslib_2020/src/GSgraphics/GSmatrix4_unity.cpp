#include <GStype.h>
#include <GSmatrix4.h>
#include <GSmathf.h>

// 拡大縮小
GSmatrix4 GSmatrix4::scale(const GSvector3 & vector) {
	return GSmatrix4{
		vector.x, 0.0f, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f, 0.0f,
		0.0f, 0.0f, vector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

// x軸回転
GSmatrix4 GSmatrix4::rotateX(float degree) {
	float rad = degree * GSmathf::deg_to_rad();
	float cos = std::cos(rad);
	float sin = std::sin(rad);
	return GSmatrix4{
		1.0f,  0.0f, 0.0f, 0.0f,
		0.0f,   cos,  sin, 0.0f,
		0.0f,  -sin,  cos, 0.0f,
		0.0f,  0.0f, 0.0f, 1.0f
	};
}

// y軸回転
GSmatrix4 GSmatrix4::rotateY(float degree) {
	float rad = degree * GSmathf::deg_to_rad();
	float cos = std::cos(rad);
	float sin = std::sin(rad);
	return GSmatrix4{
		 cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		 sin, 0.0f,  cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

// z軸回転
GSmatrix4 GSmatrix4::rotateZ(float degree) {
	float rad = degree * GSmathf::deg_to_rad();
	float cos = std::cos(rad);
	float sin = std::sin(rad);
	return GSmatrix4{
		 cos,  sin, 0.0f, 0.0f,
		-sin,  cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

// 回転
GSmatrix4 GSmatrix4::rotate(const GSquaternion& rotate) {
	float xx = rotate.x * rotate.x * 2.0f;
	float yy = rotate.y * rotate.y * 2.0f;
	float zz = rotate.z * rotate.z * 2.0f;
	float xy = rotate.x * rotate.y * 2.0f;
	float xz = rotate.x * rotate.z * 2.0f;
	float yz = rotate.y * rotate.z * 2.0f;
	float wx = rotate.w * rotate.x * 2.0f;
	float wy = rotate.w * rotate.y * 2.0f;
	float wz = rotate.w * rotate.z * 2.0f;
	return GSmatrix4{
		1.0f - yy - zz, xy + wz, xz - wy, 0.0f,
		xy - wz, 1.0f - xx - zz, yz + wx, 0.0f,
		xz + wy, yz - wx, 1.0f - xx - yy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

// 平行移動
GSmatrix4 GSmatrix4::translate(const GSvector3 & position) {
	return  GSmatrix4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f
	};
}

// この行列を移動、回転やスケーリングする行列に設定します。
GSmatrix4 GSmatrix4::TRS(const GSvector3& p, const GSquaternion& q, const GSvector3 & s) {
	GSmatrix4 result = rotate(q);
	result.m[0][0] *= s.x; result.m[0][1] *= s.x; result.m[0][2] *= s.x;
	result.m[1][0] *= s.y; result.m[1][1] *= s.y; result.m[1][2] *= s.y;
	result.m[2][0] *= s.z; result.m[2][1] *= s.z; result.m[2][2] *= s.z;
	result.m[3][0] = p.x; result.m[3][1] = p.y; result.m[3][2] = p.z;
	return result;
}

// 視野変換行列 (DirectX 左手座標系)
GSmatrix4 GSmatrix4::lookAt(const GSvector3 & eye, const GSvector3 & at, const GSvector3 & up) {
	GSmatrix4 result;
	gsMatrix4LookAtRH(&result, &eye, &at, &up);
	return result;
}

// 透視変換行列 (DirectX 左手座標系)
GSmatrix4 GSmatrix4::perspective(float fov, float aspect, float zn, float zf) {
	GSmatrix4 result;
	gsMatrix4PerspectiveRH(&result, fov, aspect, zn, zf);
	return result;
}

// 座標変換（通常）
GSvector3 GSmatrix4::multiplyPoint(const GSvector3 & point) const {
	float x = point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0] + m[3][0];
	float y = point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1] + m[3][1];
	float z = point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2] + m[3][2];
	float w = point.x * m[0][3] + point.y * m[1][3] + point.z * m[2][3] + m[3][3];
	return GSvector3{ x / w, y / w, z / w };
}

// 座標変換（高速）
GSvector3 GSmatrix4::multiplyPoint4x3(const GSvector3 & point) const {
	float x = point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0] + m[3][0];
	float y = point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1] + m[3][1];
	float z = point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2] + m[3][2];
	return GSvector3{ x, y, z };
}

// ベクトル変換（平行移動なし）
GSvector3 GSmatrix4::multiplyVector(const GSvector3 & point) const {
	float x = point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0];
	float y = point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1];
	float z = point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2];
	return GSvector3{ x, y, z };
}

// スケール値を取得
GSvector3 GSmatrix4::scale() const {
	float sx = GSvector3{ m[0][0], m[0][1], m[0][2] }.magnitude();
	float sy = GSvector3{ m[1][0], m[1][1], m[1][2] }.magnitude();
	float sz = GSvector3{ m[2][0], m[2][1], m[2][2] }.magnitude();
	return GSvector3{ sx, sy, sz };
}

// 回転の取得
GSquaternion GSmatrix4::rotation() const {
	GSquaternion result;
	float tr = m[0][0] + m[1][1] + m[2][2] + 1.0f;
	if (tr >= 1.0f) {
		float fourD = 2.0f * std::sqrt(tr);
		result.x = (m[1][2] - m[2][1]) / fourD;
		result.y = (m[2][0] - m[0][2]) / fourD;
		result.z = (m[0][1] - m[1][0]) / fourD;
		result.w = fourD / 4.0f;
		return result;
	}
	int i = 0;
	if (m[0][0] <= m[1][1]) {
		i = 1;
	}
	if (m[2][2] > m[i][i]) {
		i = 2;
	}
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	tr = m[i][i] - m[j][j] - m[k][k] + 1.0f;
	float fourD = 2.0f * std::sqrt(tr);
	result[i] = fourD / 4.0f;
	result[j] = (m[j][i] + m[i][j]) / fourD;
	result[k] = (m[k][i] + m[i][k]) / fourD;
	result[3] = (m[j][k] - m[k][j]) / fourD;
	return result;
}

// 前方向ベクトルを取得
GSvector3 GSmatrix4::forward() const {
	return GSvector3{ m[2][0], m[2][1], m[2][2] }.normalized();
}

// 上方向ベクトルを取得
GSvector3 GSmatrix4::up() const {
	return GSvector3{ m[1][0], m[1][1], m[1][2] }.normalized();
}

// 左方向ベクトルを取得
GSvector3 GSmatrix4::left() const {
	return GSvector3{ m[0][0], m[0][1], m[0][2] }.normalized();
}

// 座標を取得
GSvector3 GSmatrix4::position() const {
	return GSvector3{ m[3][0], m[3][1], m[3][2] };
}

//// この行列の逆行列。(Read Only)
//GSmatrix4 GSmatrix4::inverse() const {
//	float a0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
//	float a1 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
//	float a2 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
//	float a3 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
//	float a4 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
//	float a5 = m[0][2] * m[1][3] - m[0][3] * m[1][2];
//	float b0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
//	float b1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
//	float b2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
//	float b3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
//	float b4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
//	float b5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
//	float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
//	if (det == 0.0f) {
//		return *this; // 逆行列が存在しない！
//	}
//	float invDet = 1.0f / det;
//	return GSmatrix4{
//		(m[1][1] * b5 - m[1][2] * b4 + m[1][3] * b3) * invDet,
//		(-m[0][1] * b5 + m[0][2] * b4 - m[0][3] * b3) * invDet,
//		(m[3][1] * a5 - m[3][2] * a4 + m[3][3] * a3) * invDet,
//		(-m[2][1] * a5 + m[2][2] * a4 - m[2][3] * a3) * invDet,
//		(-m[1][0] * b5 + m[1][2] * b2 - m[1][3] * b1) * invDet,
//		(m[0][0] * b5 - m[0][2] * b2 + m[0][3] * b1) * invDet,
//		(-m[3][0] * a5 + m[3][2] * a2 - m[3][3] * a1) * invDet,
//		(m[2][0] * a5 - m[2][2] * a2 + m[2][3] * a1) * invDet,
//		(m[1][0] * b4 - m[1][1] * b2 + m[1][3] * b0) * invDet,
//		(-m[0][0] * b4 + m[0][1] * b2 - m[0][3] * b0) * invDet,
//		(m[3][0] * a4 - m[3][1] * a2 + m[3][3] * a0) * invDet,
//		(-m[2][0] * a4 + m[2][1] * a2 - m[2][3] * a0) * invDet,
//		(-m[1][0] * b3 + m[1][1] * b1 - m[1][2] * b0) * invDet,
//		(m[0][0] * b3 - m[0][1] * b1 + m[0][2] * b0) * invDet,
//		(-m[3][0] * a3 + m[3][1] * a1 - m[3][2] * a0) * invDet,
//		(m[2][0] * a3 - m[2][1] * a1 + m[2][2] * a0) * invDet
//	};
//}
//
//// この行列の転置行列
//GSmatrix4 GSmatrix4::transpose() const {
//	GSmatrix4 result;
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			result.m[i][j] = m[j][i];
//		}
//	}
//	return result;
//}

// この行列を移動、回転やスケーリングする行列に設定します。
void GSmatrix4::setTRS(const GSvector3& p, const GSquaternion& q, const GSvector3& s) {
	*this = TRS(p, q, s);
}
