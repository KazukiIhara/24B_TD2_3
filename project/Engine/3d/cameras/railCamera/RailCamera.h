#pragma once

#include <vector>

#include "3d/cameras/camera/Camera.h"
#include "math/function/MathFunction.h"

class RailCamera :public Camera {
public:
	RailCamera() = default;
	~RailCamera() = default;

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;

	// カメラをレール上に動かす
	void RunRail();

	// コントロールポイント挿入
	void PushBackControlPoint(const Vector3& point);

private:
	// スプライン曲線制御点
	std::vector<Vector3> controlPoints_;
	// レールカメラ補間変数
	float t_ = 0.0f;
	// カメラが向くtの値
	float targetOffset_ = 0.01f;
	// レールカメラの速度
	float speed_ = 0.0005f;
	// カメラの上方向
	Vector3 currentUp_{ 0.0f,1.0f,0.0f };
	float angleDifferenceX_;
};
