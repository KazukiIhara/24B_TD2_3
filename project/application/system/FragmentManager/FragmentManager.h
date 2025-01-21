#pragma once

#include "objects/Fragment/Fragment.h"

#include <vector>
#include <list>

class Earth;

// かけらマネージャ
class FragmentManager {
public:
	void Initialize();

	void Update();

	void AddFragment(const Vector3& popTranslate);

	void AddColliderList();

	void SetEarth(Earth* earth);

private:
	// かけらのリスト
	std::list<std::unique_ptr<Fragment>> fragments_;

	int32_t popTimer_ = 0;

	int32_t popIntervalTime_ = 30;

	uint32_t currentSerialNumber_ = 0;


private:
	Earth* earth_ = nullptr;

};