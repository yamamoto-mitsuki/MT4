#include <math.h>
#include <cassert>
#include "Vector3.h"

// --- 比較演算子 ---
// 代入
Vector3& Vector3::operator =(const Vector3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
// 等しいか
bool Vector3::operator ==(const Vector3& v) const {
	return x == v.x && y == v.y && z == v.z;
}
bool Vector3::operator !=(const Vector3& v) const {
	return !(*this == v);
}

// --- 四則演算 ---
// 加算
Vector3 Vector3::operator +(const Vector3& v) const {
	return Vector3(x + v.x, y + v.y, z + v.z);
}
Vector3& Vector3::operator +=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return v1 + v2;
}
// 減算
Vector3 Vector3::operator -(const Vector3& v) const {
	return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3& Vector3::operator -=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return v1 - v2;
}
// スカラー倍
Vector3 Vector3::operator*(float scalar) const {
	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3& Vector3::operator *=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
Vector3 Multiply(float scalar, const Vector3& v) {
	return v * scalar;
}
Vector3 operator*(float scalar, const Vector3& v) {
	return v * scalar;
}
// スカラー割
Vector3 Vector3::operator/(float scalar) const {
	return Vector3(x / scalar, y / scalar, z / scalar);
}
Vector3& Vector3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

// --- 幾何学計算 ---
// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
// 外積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

// 長さ(ノルム)
float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
// 正規化
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	assert(length > 0.0f && "正規化で0除算が発生しました。");
	return Vector3(v.x / length, v.y / length, v.z / length);
}

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 result(0, 0, 0);
	float dotV1V2 = Dot(v1, v2);
	float lengthSqV2 = Dot(v2, v2);
	assert(lengthSqV2 > 0.0f && "正射影ベクトルで0除算が発生しました。");
	
	float scalar = dotV1V2 / lengthSqV2;

	return v2 * scalar;
}

// 線形補完
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;
	result.z = (1.0f - t) * v1.z + t * v2.z;

	return result;
}