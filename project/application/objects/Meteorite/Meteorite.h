#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

#include <optional>

class Moon;
class FragmentManager;
class DamagePieceManager;
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

	void SetEarth(Moon* earth);
	void SetPlayer(Player* earth);

	void SetFragmentManager(FragmentManager* fragmentManager);

	void SetDamagePieceManager(DamagePieceManager* damagePieceManager);

	bool GetIsAlive()const;

	// 更新
	void Update()override;


	void MoveLimit();

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	void RootInitialize();
	void RootUpdate();

	void DamageInitialize(float damage);
	void DamageUpdate();

	void BreakInitialize();
	void BreakUpdate();

	// 大気圏内処理
	void Atmosphere();

	void SetSpeed(float speed);

	void SetPraticle(int count);

	void SetVelocity(const Vector3& velocity);

	void KillMe();

	void SetIsPop(bool isPop);

private:

	void SwitchingHPModel();

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

private:
	// 
	// パラメータ
	// 

	// 速度
	float speed_ = 0.001f;
	// 
	Vector3 velocity_{};
	// ふるまい管理変数
	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 欠片出現時間
	float emitTime_ = 0;

	// HP
	float hp_ = 3;

	// 
	float damagePieceTime_ = 0;
	const float kDamagePieceTime_ = 1.0f;


	int score_ = 500;


	float stageWidth_ = 48.0f;
	float stageHeight_ = 27.0;

	// ダメージ時間定数
	const int32_t kDamageTime_ = 30;
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
	// 大気圏範囲
	float atmosphereRenge = 10.0f;

	// 番号
	uint32_t particleNumber_ = 0;

	bool isPop_ = true;

	int timerP_ = 0;
private:
	Moon* moon_ = nullptr;

	Player* player_ = nullptr;

	FragmentManager* fragmentManager_ = nullptr;
	DamagePieceManager* damagePieceManager_ = nullptr;

	std::unique_ptr<EmitterController> emitter_;
	std::unique_ptr<EmitterController> emitterDust_;

	std::unique_ptr<EmitterController> emitterExplosionFire_;
	std::unique_ptr<EmitterController> emitterExplosionFireYellow_;
	std::unique_ptr<EmitterController> emitterExplosionDust_;


	std::unique_ptr<EmitterController> emitterDustRed_; // 赤
	std::unique_ptr<EmitterController> emitterDustYellow_; // 黄色
	std::unique_ptr<EmitterController> emitterDustGray_; // 灰色
	std::unique_ptr<EmitterController> emitterDustBlack_; // 黒っぽい色

};