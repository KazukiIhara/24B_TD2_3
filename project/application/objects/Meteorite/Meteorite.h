#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Earth;
class FragmentManager;

class Meteorite: public EntityController {
public:
	enum class Behavior {
		kRoot,
		kDagame,
		kBreak,
	};
public:
	Meteorite() = default;
	~Meteorite() = default;

	void Initialize(const std::string& name)override;

	void SetEarth(Earth* earth);

	void SetFragmentManager(FragmentManager* fragmentManager);

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	void RootInitialize();
	void RootUpdate();

	void DamageInitialize();
	void DamageUpdate();

	void BreakInitialize();
	void BreakUpdate();

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

	// 欠片出現時間
	float emitTime_ = 0;

	// HP
	uint32_t hp_ = 3;
	
	// 生存フラグ
	bool isAllive_ = true;

private:
	Earth* earth_ = nullptr;

	FragmentManager* fragmentManager_ = nullptr;
};