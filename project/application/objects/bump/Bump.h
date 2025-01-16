#pragma once

#include "3d/entityController/EntityController.h"

// たんこぶクラス
class Bump:public EntityController {
public:
	Bump() = default;
	~Bump() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	// 移動処理
	void Move();
	// 移動制限と反射処理
	void MoveLimit();
	// レベル関係
	void LevelScaling();

public: // ゲッター
	bool GetAlive() const { return isAlive_; };

	// 

private:
	// シリアルナンバー
	uint32_t serialNumber_ = 0;

	// 大きさ
	float scale_ = 1.0f;
	// Alive_
	bool isAlive_ = true;

	// 吸着しているか
	bool isAttachment_ = false;

	// たんこぶレベル
	int  level_ = 1;
	const int MaxLevel = 3;
	
	// ダメージ倍率
	float damageMultiplier_ = 1.0f;

	// 反発倍率
	float bounceFactor_ = 1.0f;

};