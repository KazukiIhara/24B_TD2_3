#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

#include <optional>


class Earth;
class Player;

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
	// 移動制限と反射処理
	void MoveLimit();
	// 生死処理
	void UpdateLifeState();
	// 大気圏内処理
	void Atmosphere();


	void SetSpeed(float speed);

	Vector3 CalculateDirection(const Vector3& startPosition, const Vector3& targetPosition, float spreadAngle);

	void SetPlayer(Player* player) { player_ = player; };

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
	void CreateEmit(const std::string emitPraticle,const std::string emitName,int count, float size,Vector3 color,EmitterController* emit);

	void DustEmit();

	void EmitFragment(const Vector3& velo);

	static Vector3 ElementWiseMax(const Vector3& a, const Vector3& b) {
		return Vector3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));
	}

	static Vector3 ElementWiseMin(const Vector3& a, const Vector3& b) {
		return Vector3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));
	}
public: // ゲッター
	bool GetAlive() const { return isAlive_; };

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
	float playerHitTimer_ = 0;

	Vector3 velocity_{};

	// 移動できる範囲
	float stageWidth_ = 48.0f;
	float stageHeight_ = 27.0;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;

	//
	uint32_t particleNumber_ = 0;
	
	// HP
	float HP_ = 3;
	// Alive_
	bool isAlive_ = true;
	float damage = 0.0f;

	// 大気圏範囲
	float atmosphereRenge = 10.0f;
private:
	Earth* earth_ = nullptr;

	Player* player_ = nullptr;

	// Emitterコントローラ
	std::unique_ptr<EmitterController> emitter_;
	std::unique_ptr<EmitterController> emitterDust_;
	
	std::unique_ptr<EmitterController> emitterFragment_;

	
};