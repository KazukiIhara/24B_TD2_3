#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Earth;

// 隕石のかけら
class Fragment : public EntityController {
public:
	enum class Behavior {
		kRoot,
	};
public:
	Fragment() = default;
	~Fragment() = default;

	void Initialize(const std::string& name)override;

	void SetEarth(Earth* earth);

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	void HitTimerUpdate();

	void BehaviorChange();
	void BehaviorUpdate();
	
	void RootInitialize();
	void RootUpdate();

	void Move();

	void SetSpeed(float speed);
private:
	// 
	// パラメータ
	// 

	// 速度
	float speed_ = 0.001f;

	// ふるまい管理変数
	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


	int32_t kNoneHitTime_ = 2;
	int32_t playerHitTimer_ = 0;

	Vector3 velocity_{};

private:
	Earth* earth_ = nullptr;
};