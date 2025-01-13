#pragma once

#include "3d/entityController/EntityController.h"

class Player: public EntityController {
public:
	Player() = default;
	~Player() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// グローバルデータから値を取得
	void SetParamaters();

	// 操作
	void Operation();
	// 移動処理
	void Move();

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	// 定数
	const std::string kParamaterString = "PlayerParamater";

	// パラメータ

	// 初期化位置
	Vector3 initializePosition_{};

	// 速度定数
	float kSpeed_ = 0.01f;
	// 最大速度(std::clampで利用しているため、-の値にならないように注意)
	float kMaxSpeed_ = 0.2f;

	// 移動ベクトル
	Vector3 moveVector_{};
	// 加速量
	Vector3	acceralation_{};
	// 移動量
	Vector3 velocity_{};
};