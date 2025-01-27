#pragma once
#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

#include <optional>


class DamagePiece  : public EntityController
{
public:

	void Initialize(const std::string& name)override;

	void InitVelocity(const Vector3 velo) { velocity_ = velo; };

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	
public: // ゲッター
	bool GetAlive() const { return isAlive_; };

	void SetIsReduction() {isReduction = true; }

private:
	void Move();

	void ScaleReduction();


	static	float Clamp(float t, float min, float max)
	{
		if (t < min)
			return min;
		if (t > max)
			return max;
		return t;
	}

private:
	// 破壊時間定数
	const float kMaxBreakTime_ = 6;
	const float kMinBreakTime_ = 3;
	float kBreakTime_ = 0;

	// 破壊タイマー
	float breakTimer_ = 0;
	// Alive_
	bool isAlive_ = true;
	bool isReduction = false;


	Vector3 velocity_{};

	float scaleMax = 0.5f;
	float scaleMin= 0.1f;

	float scale_ = 0;
};

