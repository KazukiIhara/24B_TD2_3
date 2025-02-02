#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class BumpManager;

class Moon;

class Player: public EntityController {
	enum class Behavior {
		kRoot,
		kCharge,
		kThrowMoon,
	};
public:
	Player() = default;
	~Player() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// グローバルデータから値を取得
	void SetParamaters();

	// タイマー処理
	void HitTimersUpdate();

	// 操作
	void Operation();
	// 移動処理
	void Move();

	// 移動制限
	void MoveLimit();

	// 月射出
	void Shot();

	// 生死処理
	void UpdateLifeState();


	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	Vector3 RotatePosition(const Vector3& position, float angle);

	WorldTransform* GetLocalTransform();

	void BehaviorUpdate();
	void RootInitialize();
	void RootUpdate();
	void ChargeInitialize();
	void ChargeUpdate();
	void ThrowInitialize();
	void ThrowUpdate();

public:
	void SetMoon(Moon* moon) {
		moon_ = moon;
	}

	void SetBumpManager(BumpManager* bumpManager) {
		bumpManager_ = bumpManager;
	};

	bool GetIsHit() const {
		return isObjectHit;
	};

	void SetIsHit(bool hit) {
		isObjectHit = hit;
	}

	int GetHitLevel() {
		return objectHitLevel;
	}
	
	float& GetHp();
private:

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorldTransform localTransform_{};

	// 定数
	const std::string kParamaterString = "PlayerParamater";

	// パラメータ

	// 初期化位置
	Vector3 initializePosition_{};

	// 速度定数
	float kSpeed_ = 0.01f;
	// 最大速度(std::clampで利用しているため、-の値にならないように注意)
	float kMaxSpeed_ = 0.2f;
	// 回転速度
	float rotationSpeed_ = std::numbers::pi_v<float>;
	// 大きさ
	float scale_ = 1.0f;

	// 月の回転速度
	float moonRotationSpeed_ = std::numbers::pi_v<float>*2.0f;

	// 移動できる範囲
	float stageWidth_ = 32.0f;
	float stageHeight_ = 18.0f;

	// 移動ベクトル
	Vector3 moveVector_{};
	// 加速量
	Vector3	acceralation_{};
	// 移動量
	Vector3 velocity_{};

	// 回転方向
	float rotateDirection_ = 0;

	// 一度当たった後に当たらない時間
	int32_t kNoneHitTime_ = 2;
	// 対地球ヒットタイマー
	int32_t earthHitTimer_ = 0;
	// 対隕石ヒットタイマー
	int32_t meteoriteHitTimer_ = 0;

	// 対隕石たんこぶ生成ヒットタイマー
	float meteoriteBumpPopHitTimer_ = 0;
	// たんこぶのできる位置
	Vector3 bumpDirection_{};

	BumpManager* bumpManager_ = nullptr;

	// 一周するフレーム
	const float aroundFrame_ = 300.0f;
	float inclination_ = 23.4f;
	float inclinationRadian_ = 0.0f;



	// 月をキャッチする距離
	float moonCatchDistance_ = 5.0f;

	// キャッチする距離と月が実際に回転する距離のオフセット
	float moonRotateDistanceOffset_ = 0.5f;

	const int32_t catchTime_ = 30;
	int32_t catchTimer_ = 0;

	float HP_ = 100.0f;
	bool isAlive_ = true;
	// 当たった物体の大きさ(これでカメラのシェイクの大きさが決まる)
	int objectHitLevel = 0;
	bool isObjectHit = false;



	Moon* moon_ = nullptr;

};