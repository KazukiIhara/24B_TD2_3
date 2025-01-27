#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

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

	void SetParticle(int count);
private:
	// スケール
	void ScaleAmimetion();
	//

	float easeInBounce(float t);

	float easeOutBounce(float t);
private:
	// シリアルナンバー
	uint32_t serialNumber_ = 0;

	// 大きさ
	float scale_ = 1.0f;

	float scalelevel0_ = 0.0f;
	float scalelevel1_ = 0.5f;
	float scalelevel2_ = 0.7f;
	float scalelevel3_ = 1.0f;

	float scaleTimer_ = 0;

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



	bool isScaleUp = true;
	float animetionTimer_ = 0.0f;
	const float kAnimetionTimer_ = 1.0f;

	float targetScale_ = 0.0f;
	float previousScale_ = 0.0f;


	std::unique_ptr<EmitterController> emitter_;


};