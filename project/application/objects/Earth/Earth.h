#pragma once

#include "3d/entityController/EntityController.h"

class Earth: public EntityController {
public:
	Earth() = default;
	~Earth() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;


private:

};