#pragma once

#include "3d/entityController/EntityController.h"

//#include "system/BumpManager/BumpManager.h

class BumpManager;

class Player: public EntityController {
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

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	Vector3 RotatePosition(const Vector3& position, float angle);

public:

	void SetBumpManager(BumpManager* bumpManager) {
		bumpManager_ = bumpManager;
	};

private:
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
	float rotationSpeed_ = 0.1f;
	// 大きさ
	float scale_ = 1.0f;

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
};