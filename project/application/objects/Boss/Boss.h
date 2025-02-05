#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Boss: public EntityController {
	enum class Behavior {
		kNone,
		kIn,
		kRoot,
		kDamage,
		kBreak,
	};
public:
	Boss();
	~Boss();

	void Initialize(const std::string& name)override;
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;


	void RequestIn();
	void RequestRoot();

	void InInitialize();
	void InUpdate();

	void RootInitialize();
	void RootUpdate();

	void DamageInitialize();
	void DamageUpdate();

	void BreakInitialize();
	void BreakUpdate();

	bool IsBossKill() const;

	void AddColliderList();
private:

	int32_t hp_ = 3;

	Behavior behavior_ = Behavior::kNone;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	Vector3 velocity_{};

	uint32_t damageTimer_ = 0;
	uint32_t damageTime_ = 30;

	uint32_t breakTimer_ = 0;
	uint32_t breakTime_ = 30;

	bool isBossKill_ = false;

};