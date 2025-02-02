#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class UFO: public EntityController {
	enum class Behavior {
		kRoot,
	};
public:
	UFO();
	~UFO();

	void Initialize(const std::string& name)override;

	bool GetIsAlive()const;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	void RootInitialize();
	void RootUpdate();
private:
	// 
	// パラメータ
	// 

	// 速度
	float speed_ = 0.001f;

	bool isAlive_ = true;


	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


};
