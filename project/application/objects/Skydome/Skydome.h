#pragma once

#include "3d/entityController/EntityController.h"

class Skydome : public EntityController {
public:
	Skydome() = default;
	~Skydome() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	// スピード
	float speed_ = 0.0001f;

};