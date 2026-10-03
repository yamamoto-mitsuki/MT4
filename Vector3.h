#pragma once

class Vector3 {
public:
	float x, y, z;

	// デフォルトコンストラクタ
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

	// コピーコンストラクタ
	Vector3(const Vector3& a) : x(a.x), y(a.y), z(a.z) {}
	// 3つの値で作成
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	// --- 比較演算 ---
	// 代入
	Vector3& operator =(const Vector3& v);
	// 等しいか
	bool operator ==(const Vector3& v) const;
	bool operator !=(const Vector3& v) const;

	// --- 四則演算 ---
	// 加算
	Vector3  operator +(const Vector3& v) const;
	Vector3& operator +=(const Vector3& v);
	// 減算
	Vector3  operator -(const Vector3& v) const;
	Vector3& operator -=(const Vector3& v);
	// スカラー倍
	Vector3  operator *(float scalar) const;
	Vector3& operator *=(float scalar);
	// スカラー割
	Vector3  operator/(float scalar) const;
	Vector3& operator/=(float scalar);
};

// --- 単項演算子 ---
inline Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }

// --- 四則演算 ---
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
Vector3 operator*(float scalar, const Vector3& v);

// --- 幾何学的計算 ---
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 外積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 長さ(ノルム)
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

// 線形補完
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);