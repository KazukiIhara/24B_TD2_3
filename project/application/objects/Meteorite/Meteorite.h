#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Earth;
class FragmentManager;
class Player;

class Meteorite : public EntityController {
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
	void SetPlayer(Player* earth);

	void SetFragmentManager(FragmentManager* fragmentManager);

	bool GetIsAlive()const;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	void RootInitialize();
	void RootUpdate();

	void DamageInitialize(float damage);
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
	float hp_ = 3;



	// ダメージ時間定数
	const int32_t kDamageTime_ = 10;
	// ダメージタイマー
	int32_t damageTimer_ = 0;

	// 破壊時間定数
	const int32_t kBreakTime_ = 30;
	// 破壊タイマー
	int32_t breakTimer_ = 0;

	// 生存フラグ
	bool isAlive_ = true;

	//
	float damege_;

private:
	Earth* earth_ = nullptr;

	Player* player_ = nullptr;

	FragmentManager* fragmentManager_ = nullptr;
};