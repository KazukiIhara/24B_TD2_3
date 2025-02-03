#pragma once

#include "objects/UFOBullet/UFOBullet.h"

#include <list>

class Player;

class UFOBulletManager {
public:
	void Initialize();

	void Update();

	void AddUFOBullet(const Vector3& translate, const Vector3& velocity);
	void AddColliderList();

	void SetPlayer(Player* player);

private:
	std::list<std::unique_ptr<UFOBullet>> ufoBullets_;
	uint32_t currentSerialNumber_ = 0;
private:
	Player* player_ = nullptr;
};