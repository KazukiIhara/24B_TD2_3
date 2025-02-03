#pragma once

#include "3d/entityController/EntityController.h"


class UFOBullet :public EntityController {
public:
	UFOBullet();
	~UFOBullet();

	void Initialize(const std::string& name)override;
	// 更新
	void Update()override;
	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;
	void SetVelocity(const Vector3& velocity);
	bool GetIsAlive()const;
	void MoveLimit();
private:
	Vector3 velocity_ = {};
	bool isAlive_ = true;

	float stageWidth_ = 48.0f;
	float stageHeight_ = 27.0;
};
