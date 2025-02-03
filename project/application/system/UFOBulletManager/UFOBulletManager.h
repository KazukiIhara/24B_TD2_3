#pragma once

#include "objects/UFOBullet/UFOBullet.h"

#include <list>

class Player;

class UFOBulletManager {
public:
	void Initialize(Player* player);

	void Update();

	void AddUFOBullet(const Vector3& translate);
	void AddColliderList();

	void SetPlayer(Player* player);

private:
	std::list<std::unique_ptr<UFOBullet>> ufoBullets_;
	uint32_t currentSerialNumber_ = 0;
	float speed_ = 1.0f;
private:
	Player* player_ = nullptr;
};