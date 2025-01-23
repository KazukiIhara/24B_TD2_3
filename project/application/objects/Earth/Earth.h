#pragma once

#include "3d/entityController/EntityController.h"

class Earth : public EntityController {
public:
	Earth() = default;
	~Earth() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;


	// 移動制限
	void MoveLimit();


	// 初期位置に戻る
	void ReturnPosition();

	// 生死処理
	void UpdateLifeState();

	float& GetHp();

private:
	Vector3 velocity_{};

	// 移動できる範囲
	float stageWidth_ = 31.0f;
	float stageHeight_ = 17.0f;

	// 初期位置に戻ろうとする行動に入る時間
	float kReturnMoveTime_ = 3.0f;
	float returnMoveTimer_ = 0.0f;
	float returnSpeed_ = 0.9f;

	float HP_ = 100.0f;

	bool isAlive_ = true;

	float inclination_ = 23.4f;
	float inclinationRadian_ = 0.0f;
};