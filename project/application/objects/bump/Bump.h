#pragma once

#include "3d/entityController/EntityController.h"

// たんこぶクラス
class Player;

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
	// 生死処理
	void UpdateLifeState();
	

public: // ゲッター
	bool GetAlive() const { return isAlive_; }; // 生死

	//float GetDamageMultiplier() const { return damageMultiplier_;} // ダメージ倍率

	//float GetBounceFactor() const { return bounceFactor_; } // 反発倍率;

	//
	void GrowthBump() { experiencePoints_++; };

	// 
public: // セッター
	void SetPlayer(Player* player) { player_ = player; };

private:
	// シリアルナンバー
	uint32_t serialNumber_ = 0;

	// 大きさ
	float scale_ = 1.0f;
	// Alive_
	bool isAlive_ = true;

	// たんこぶレベル
	int  level_ = 1;
	
	const int MaxLevel = 3;

	// 経験値
	float experiencePoints_; 
	
	// ダメージ倍率
	float damageMultiplier_ = 1.0f;

	// 反発倍率
	float bounceFactor_ = 1.0f;

	// プレイヤ
	Player* player_ = nullptr; 

	// 無敵時間
	float maxInvincibilityTime_ = 2.0f;
	float invincibilityTimer_ = 0.0f;


};