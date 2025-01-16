#pragma once

#include "objects/bump/Bump.h"

#include "list"

class Player;

// たんこぶマネージャ
class BumpManager {
public:
	BumpManager() = default;
	~BumpManager() = default;

	void Initialize();

	void Update();

	void AddBump(const Vector3& popTranslate);

	void AddColliderList();
private:
	// たんこぶのリスト
	std::list<std::unique_ptr<Bump>> bumps_;


	uint32_t currentSerialNumber_ = 0;
};