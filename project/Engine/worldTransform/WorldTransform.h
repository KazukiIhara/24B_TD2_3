#pragma once

// C++
#include <cmath>
#include <numbers>

// MyHedder
#include "math/function/MathFunction.h"

// ワールドトランスフォームクラス
class WorldTransform {
public: // メンバ関数
	WorldTransform() = default;
	~WorldTransform() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();

private: // コピー禁止
	// コピー禁止
	WorldTransform(const WorldTransform&) = delete;
	WorldTransform& operator=(const WorldTransform&) = delete;

public: // メンバ変数
	// ローカルトランスフォーム
	Vector3 scale_ = { 1.0f,1.0f,1.0f };// 拡縮量
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };// 回転量
	Vector3 translate_ = { 0.0f,0.0f,0.0f };// 移動量

	// 親
	WorldTransform* parent_;

	// ワールド行列
	Matrix4x4 worldMatrix_{};
};