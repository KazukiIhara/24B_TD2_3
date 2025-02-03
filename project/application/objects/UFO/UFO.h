#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class UFOBulletManager;

class UFO : public EntityController {
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

	void SetVelocity(const Vector3& velocity);

	void SetUFOBulletManager(UFOBulletManager* ufoBulletManager);

private:
	// 
	// パラメータ
	// 
	float time_ = 0.1f;

	Vector3 initVelocity_ = {};
	Vector3 velocity_ = {};

	float phase_ = 0.0f;

	// 速度
	float speed_ = 0.001f;

	float amplitude_ = 3.0f;  // 揺れの大きさ
	float frequency_ = 0.5f;  // 揺れの速さ


	bool isAlive_ = true;


	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private:
	UFOBulletManager* ufoBulletManager_ = nullptr;

};
