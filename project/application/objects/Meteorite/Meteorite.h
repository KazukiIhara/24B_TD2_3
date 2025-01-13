#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Earth;

class Meteorite: public EntityController {
public:
	enum class Behavior {
		kRoot,
	};
public:
	Meteorite() = default;
	~Meteorite() = default;

	void Initialize(const std::string& name)override;

	void SetEarth(Earth* earth);

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;


	void RootInitialize();
	void RootUpdate();


	// ふるまい管理変数
	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private:
	Earth* earth_ = nullptr;
};