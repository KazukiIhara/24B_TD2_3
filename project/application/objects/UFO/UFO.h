#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

#include <optional>

class UFOBulletManager;
class DamagePieceManager;
class Player;

class UFO : public EntityController {
	enum class Behavior {
		kRoot,
		kDamage,
		kBreak,
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

	void DamageInitialize();
	void DamageUpdate();

	void BreakInitialize();
	void BreakUpdate();

	void SetVelocity(const Vector3& velocity);

	void SetUFOBulletManager(UFOBulletManager* ufoBulletManager);

	void MoveLimit();

	void SetDamagePieceManager(DamagePieceManager* damagePieceManager);


	void KillMe();

	void SetPlayer(Player* player);


	void SetPraticle(int count);

private:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="emitPraticle"></param> パーティクル名
	/// <param name="emitName"></param> エミッター名
	/// <param name="count"></param> 出現数
	/// <param name="size"></param> 大きさ(float)
	/// <param name="color"></param> カラー(Vector)　
	/// <param name="emit"></param> エミッタークラス
	void CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit);

	//void EmitFragment(const Vector3& velo);
	void EmitDust(const Vector3& pos, const Vector3& veloctiy);

	void EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit);


	static Vector3 ElementWiseMax(const Vector3& a, const Vector3& b) {
		return Vector3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));
	}

	static Vector3 ElementWiseMin(const Vector3& a, const Vector3& b) {
		return Vector3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));
	}

	void ShotEllipseEmit();

	void ShotEmit();


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

	float stageWidth_ = 65.0f;
	float stageHeight_ = 35.0;


	bool isAlive_ = true;

	uint32_t hp_ = 2;

	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = Behavior::kRoot;

	int32_t shotTimer_ = 0;
	int32_t shotInterval_ = 360;

	// ダメージ時間定数
	const int32_t kDamageTime_ = 30;
	// ダメージタイマー
	int32_t damageTimer_ = 0;

	// 破壊時間定数
	const int32_t kBreakTime_ = 30;
	// 破壊タイマー
	int32_t breakTimer_ = 0;


	// 番号
	uint32_t particleNumber_ = 0;
private:
	UFOBulletManager* ufoBulletManager_ = nullptr;

	DamagePieceManager* damagePieceManager_ = nullptr;

	Player* player_ = nullptr;

	// エミッター
	std::unique_ptr<EmitterController> emitterExplosionFire_;
	std::unique_ptr<EmitterController> emitterExplosionFireYellow_;
	std::unique_ptr<EmitterController> emitterExplosionDust_;

	std::unique_ptr<EmitterController> emitterShotDust_;


};
