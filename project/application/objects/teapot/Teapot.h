#pragma once

#include "3d/entityController/EntityController.h"

class Teapot : public EntityController {
public:
	Teapot() = default;
	~Teapot() = default;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;
};