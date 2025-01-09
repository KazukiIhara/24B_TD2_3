#define NOMINMAX

#include "RailCamera.h"

#include "imgui.h"

#include <cmath> 
#include <algorithm>

void RailCamera::Initialize() {
	// 基本的なカメラの初期化
	Camera::Initialize();
	transform_.translate_ = { 0.0f,0.0f,0.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };

	// カメラの値を初期化で変更した場合、更新処理に入る前に一度行列を更新する
	Camera::Update();

	// 補間用変数をリセット
	t_ = 0.0f;
}

void RailCamera::Update() {
#ifdef _DEBUG
	ImGui::Begin("RailCamera");
	ImGui::DragFloat("t_ ", &t_, 0.001f, 0.0f, 1.0f);
	ImGui::Text("AngleDifferenceX: %f", angleDifferenceX_);
	ImGui::DragFloat3("rotate ", &transform_.rotate_.x);
	ImGui::End();
#endif // _DEBUG


	// レールを走らせる
	RunRail();

	// 基本的なカメラの更新
	Camera::Update();
}

void RailCamera::RunRail() {
	// 世界の上方向ベクトル
	constexpr Vector3 worldUp = { 0.0f, 1.0f, 0.0f };
	static Vector3 currentUp_ = worldUp;

	// t_の更新
	t_ = std::min(t_ + speed_, 1.0f);

	// 現在位置と次の位置を計算して進行方向ベクトルを取得
	transform_.translate_ = CatmullRomSpline(controlPoints_, t_);
	Vector3 nextPosition = CatmullRomSpline(controlPoints_, std::min(t_ + targetOffset_, 1.0f));
	Vector3 forward = Normalize(nextPosition - transform_.translate_);

	// 進行方向に基づく右方向と上方向を計算
	Vector3 right = Normalize(Cross(currentUp_, forward));
	Vector3 calculatedUp = Cross(forward, right);

	// X軸の角度差を計算して補正
	angleDifferenceX_ = std::fabs(std::atan2(calculatedUp.y, calculatedUp.z) - std::atan2(worldUp.y, worldUp.z)) * (180.0f / std::numbers::pi_v<float>);
	// X軸の回転角が60度を越えている場合、進行方向から上方向を算出してカメラの回転を決める
	currentUp_ = (angleDifferenceX_ < 60.0f) ? Lerp(currentUp_, worldUp, 0.1f) : calculatedUp;

	// クォータニオンで回転を設定し、オイラー角に変換
	transform_.rotate_ = QuaternionToEulerAngles(QuaternionLookRotation(forward, currentUp_));
}


void RailCamera::PushBackControlPoint(const Vector3& point) {
	controlPoints_.push_back(point);
}
