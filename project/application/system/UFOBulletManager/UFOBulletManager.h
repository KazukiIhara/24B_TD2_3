#pragma once

#include "objects/UFOBullet/UFOBullet.h"

#include <list>

class Player;
class DamagePieceManager;

class UFOBulletManager {
public:
	void Initialize(Player* player);

	void Update();

	void AddUFOBullet(const Vector3& translate);
	void AddColliderList();

	void SetPlayer(Player* player);
	void SetDamagePieceManager(DamagePieceManager* damagePieceManager);

private:
	std::list<std::unique_ptr<UFOBullet>> ufoBullets_;
	uint32_t currentSerialNumber_ = 0;
	float speed_ = 5.0f;
private:
	Player* player_ = nullptr;
	DamagePieceManager* damagePieceManager_ = nullptr;
};