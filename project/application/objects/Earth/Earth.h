#pragma once

#include "3d/entityController/EntityController.h"

class Earth: public EntityController {
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

private:
	Vector3 velocity_{};
	int32_t kNoneHitTime_ = 2;
	int32_t earthHitTimer_ = 0;

	// 移動できる範囲
	float stageWidth_ = 18.0f;
	float stageHeight_ = 9.0f;

	// 初期位置に戻ろうとする行動に入る時間
	float kReturnMoveTime_ = 5.0f;
	float returnMoveTimer_ = 0.0f;
	float returnSpeed_ = 0.25f;

};