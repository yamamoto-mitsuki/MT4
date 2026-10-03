#include <Novice.h>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <imgui.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Spherical.h"

const char kWindowTitle[] = "MT4";

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

	// ===== 初期値 =====
	const float deltaTime = 1.0f / 60.0f;
	Vector2 pos = {640.0f, 360.0f}; // 円B
	float speed = 5.0f; // 追従速度
	



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

		
		// ===== 入力 =====
		// 円A
		int mouseX, mouseY = 0;
		Novice::GetMousePosition(&mouseX, &mouseY);
		// 追従目標値
		const Vector2 target = {static_cast<float>(mouseX), static_cast<float>(mouseY)};
		// 進む
		pos.x += (speed * deltaTime) * (target.x - pos.x);
		pos.y += (speed * deltaTime) * (target.y - pos.y);

		// ===== ImGui =====
		ImGui::Begin("Interpolation Controller");
		ImGui::Text("Target: Mouse Position (Red Circle)");
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "[Linear Interpolation - Student Mode]");
		ImGui::SliderFloat("Speed", &speed, 0.0f, 60.0f, "%.1f");
		ImGui::Separator();
		// マウスと円Bの座標
		ImGui::Text("Mouse Pos: (%d, %d)", mouseX, mouseY);
		ImGui::Text("Circle Pos: (%.1f, %.1f)", pos.x, pos.y);
		// 円同士の中心間の距離
		const float dx = static_cast<float>(mouseX) - pos.x;
		const float dy = static_cast<float>(mouseY) - pos.y;
		const float distance = std::sqrt(dx * dx + dy * dy);
		ImGui::Text("Distance to Target: %.1f px", distance);
		ImGui::Separator();
		// 円Bをマウス位置へ移動
		if (ImGui::Button("Snap to Target")) {
			pos.x = static_cast<float>(mouseX);
			pos.y = static_cast<float>(mouseY);
		}
		// 次のボタンを横に並べる
		ImGui::SameLine();
		// 円Bを画面中央へ戻す
		if (ImGui::Button("Reset to Center")) {
			pos.x = 640.0f;
			pos.y = 360.0f;
		}
		ImGui::End();

		// ===== 描画 =====
		const int drawX = static_cast<int>(pos.x);
		const int drawY = static_cast<int>(pos.y);
		// 中心同士を結ぶ線。円より先に描く。
		Novice::DrawLine(mouseX, mouseY, drawX, drawY, 0xFFFFFFFF);
		// 円B
		Novice::DrawEllipse(drawX, drawY, 20, 20, 0.0f, 0x00FF00FF, kFillModeSolid);
		// 円A
		Novice::DrawEllipse(mouseX, mouseY, 12, 12, 0.0f, 0xFF0000FF, kFillModeSolid);


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
