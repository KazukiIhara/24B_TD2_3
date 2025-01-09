#pragma once

#include "3d/entityController/EntityController.h"

class PronamaChan : public EntityController {
public:
	PronamaChan() = default;
	~PronamaChan() = default;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	// 衝突フラグ
	bool hitFlag_ = false;
};