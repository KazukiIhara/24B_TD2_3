#pragma once

#include "3d/entityController/EntityController.h"

// 隕石のかけら
class Fragment: public EntityController {
public:
	Fragment() = default;
	~Fragment() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

private:

};