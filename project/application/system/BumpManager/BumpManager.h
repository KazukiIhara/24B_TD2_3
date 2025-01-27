#pragma once

#include "objects/bump/Bump.h"

#include "list"

class Player;
class Earth;

// たんこぶマネージャ
class BumpManager {
public:
	BumpManager() = default;
	~BumpManager() = default;

	void Initialize();

	void Update();

	void AddBump(const Vector3& popTranslate);

	void AddColliderList();

	void SetPlayer(Player* player) { player_ = player; };

	void SetEarth(Earth* earth) { earth_ = earth; }

private:
	// たんこぶのリスト
	std::list<std::unique_ptr<Bump>> bumps_;

	// プレイヤー
	Player* player_ = nullptr;
	Earth* earth_ = nullptr;

	uint32_t currentSerialNumber_ = 0;
};