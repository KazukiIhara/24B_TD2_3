#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>
class UFOBulletManager;

class Player;

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

	void SetUFOBulletManager(UFOBulletManager* ufobulletManager);
	void SetPlayer(Player* player);

private:

	int32_t hp_ = 10;

	Behavior behavior_ = Behavior::kNone;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	Vector3 velocity_{};

	uint32_t damageTimer_ = 0;
	uint32_t damageTime_ = 30;

	uint32_t breakTimer_ = 0;
	uint32_t breakTime_ = 30;

	float time_ = 0.0f;

	float amplitude_ = 0.2f;  // 揺れの大きさ
	float frequency_ = 0.1f;  // 揺れの速さ


	bool isBossKill_ = false;

	int32_t shotTimer_ = 0;
	int32_t shotTime_ = 300;

	UFOBulletManager* ufoBulletManager_ = nullptr;

	Player* player_ = nullptr;

};