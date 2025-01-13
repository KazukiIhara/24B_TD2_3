#pragma once

#include "3d/entityController/EntityController.h"

// たんこぶクラス
class Bump:public EntityController {
public:
	Bump() = default;
	~Bump() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	// 大きさ
	float scale_ = 1.0f;
};