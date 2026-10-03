#include <Novice.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <imgui.h>
#include "Vector3.h"
#include "Matrix4x4.h"

const char kWindowTitle[] = "MT4";

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
// 注視点（原点）を向くカメラのワールド行列を作成する
Matrix4x4 MakeCameraMatrix(const Vector3& pos) {
	// 注視点（0,0,0）への前
	Vector3 target = {0.0f, 0.0f, 0.0f};
	Vector3 worldUp = {0.0f, 1.0f, 0.0f}; // 世界の上
	Vector3 forward = Normalize(Subtract(target, pos));
	Vector3 right = Normalize(Cross(worldUp, forward));
	Vector3 up = Cross(forward, right);
	// 求めた右・上・前とカメラ位置を行列に格納
	Matrix4x4 mat;
	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[0][3] = 0.0f;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = forward.x;
	mat.m[2][1] = forward.y;
	mat.m[2][2] = forward.z;
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = pos.x;
	mat.m[3][1] = pos.y;
	mat.m[3][2] = pos.z;
	mat.m[3][3] = 1.0f;

	return mat;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// 初期値
	const float halfPi = std::numbers::pi_v<float> / 2.0f;
	Spherical s{6.0f, 0.0f, -halfPi};



	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// 球面座標から直交座標を計算
		Vector3 pos = ToCartesian(s);
		// カメラのワールド行列を作成
		Matrix4x4 cameraMatrix = MakeCameraMatrix(pos);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// ===== ImGui =====
		ImGui::Begin("Spherical Coordinates");
		ImGui::Text("Target: (0, 0, 0) / +Y up / Camera +Z forward");
		ImGui::Separator();
		// 球面座標
		ImGui::DragFloat("Radius", &s.radius, 0.01f);
		ImGui::DragFloat("Theta: elevation (rad)", &s.theta, 0.01f);
		ImGui::DragFloat("Phi (rad)", &s.phi, 0.01f);
		ImGui::Separator();
		// 変換した直交座標と、作成したカメラ行列を表示する
		ImGui::Text("Spherical: r = %.3f, theta = %.3f rad, phi = %.3f rad", s.radius, s.theta, s.phi);
		ImGui::Text("Cartesian: x = %.3f, y = %.3f, z = %.3f", pos.x, pos.y, pos.z);
		ImGui::Separator();

		ImGui::Text("Camera matrix");
		for (int i = 0; i < 4; ++i) {
			ImGui::Text("%8.3f %8.3f %8.3f %8.3f", cameraMatrix.m[i][0], cameraMatrix.m[i][1], cameraMatrix.m[i][2], cameraMatrix.m[i][3]);
		}

		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
