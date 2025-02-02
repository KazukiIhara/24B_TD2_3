#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

#include <optional>

class DamagePieceManager;
class Player;
class Moon: public EntityController {
public:
	enum class Behavior {
		kRoot,
		kAttack,
		kBack,
	};
public:
	Moon() = default;
	~Moon() = default;

	void Initialize(const std::string& name)override;

	void SetPlayer(Player* player);

	// 更新
	void Update()override;
	void UpdateTitle();

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	// 移動制限
	void MoveLimit();


	// 初期位置に戻る
	void ReturnPosition();

	
	void SetPraticle();

	

	void SetDamagePieceManager(DamagePieceManager* damagePieceManager) {
		damagePieceManager_ = damagePieceManager;
	}

	

	float GetAroundFrame() const;

	void RootRequest();
	void AttackRequest();
	void BackRequest();

	void RootInitialize();
	void RootUpdate();
	void AttackInitialize();
	void AttackUpdate();
	void BackInitialize();
	void BackUpdate();

	void SetVelocity(const Vector3& velocity);

private:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="emitPraticle"></param> パーティクル名
	/// <param name="emitName"></param> エミッター名
	/// <param name="count"></param> 出現数
	/// <param name="size"></param> 大きさ(float)
	/// <param name="lifeTime"></param> 生存時間(Vector2){x = min,y = max}
	/// <param name="color"></param> カラー(Vector)　
	/// <param name="emit"></param> エミッタークラス
	void CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit);

	void EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit);
	void EmitDamegePiece(const Vector3& pos, const Vector3& veloctiy, DamagePieceManager* damagePieceManager_);
	void EmitDamegePiece2(const Vector3& pos, const Vector3& veloctiy, DamagePieceManager* damagePieceManager_);

	void EmitDust(const Vector3& pos, const Vector3& veloctiy);

	static Vector3 ElementWiseMax(const Vector3& a, const Vector3& b) {
		return Vector3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));
	}

	static Vector3 ElementWiseMin(const Vector3& a, const Vector3& b) {
		return Vector3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));
	}
private:
	std::unique_ptr<EmitterController> emitterDustRed_; // 赤
	std::unique_ptr<EmitterController> emitterDustYellow_; // 黄色
	std::unique_ptr<EmitterController> emitterDustGray_; // 灰色
	std::unique_ptr<EmitterController> emitterDustBlack_; // 黒っぽい色


	float HitParticleTimer_ = 0;
private:
	// ふるまい
	Behavior behavior_ = Behavior::kRoot;
	// ふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 一周するフレーム
	const float aroundFrame_ = 300.0f;

	Vector3 velocity_{};

	// 移動できる範囲
	float stageWidth_ = 33.6f;
	float stageHeight_ = 18.9f;

	// 初期位置に戻ろうとする行動に入る時間
	float kReturnMoveTime_ = 3.0f;
	float returnMoveTimer_ = 0.0f;
	float returnSpeed_ = 0.9f;

	

	// 当たった物体の大きさ(これでカメラのシェイクの大きさが決まる)
	//int objectHitLevel = 0;
	//bool isObjectHit = false;

	// プレイヤーとの距離
	float speed_ = 20.0f;

	DamagePieceManager* damagePieceManager_ = nullptr;

	Player* player_ = nullptr;

	Vector3 targetPos_ = { 0.0f,0.0f,0.0f };
};