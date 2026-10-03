#pragma once
#include <cmath>
#include <algorithm>
#include "Vector3.h"

// ===== 構造体 =====
// 球面座標系
struct Spherical {
	float radius; // 動径 r
	float theta;  // 仰角 Θ
	float phi;    // 方位角 φ
};

// ===== 関数 =====
// 球面座標から直交座標への変換
Vector3 ToCartesian(const Spherical& s) {
	float rho = s.radius * std::cos(s.theta);
	return {rho * std::cos(s.phi), s.radius * std::sin(s.theta), rho * std::sin(s.phi)};
}
// 直交座標から球面座標への変換
Spherical ToSpherical(const Vector3& p) {
	float r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	if (r == 0.0f) {
		return {0.0f, 0.0f, 0.0f};
	}
	float sinTheta = std::clamp(p.y / r, -1.0f, 1.0f);
	float phi = 0.0f;
	if (p.x != 0.0f || p.z != 0.0f) {
		phi = std::atan2(p.z, p.x);
	}
	return {r, std::asin(sinTheta), phi};
}