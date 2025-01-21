#pragma once

#include "objects/Fragment/Fragment.h"

#include <vector>
#include <list>

class Earth;

class Player;

// かけらマネージャ
class FragmentManager {
public:
	void Initialize();

	void Update();

	void AddFragment(const Vector3& popTranslate, Earth* earth, Player* player);

	void AddColliderList();

private:
	// かけらのリスト
	std::list<std::unique_ptr<Fragment>> fragments_;

	uint32_t currentSerialNumber_ = 0;

};