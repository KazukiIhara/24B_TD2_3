#pragma once

#include "objects/Fragment/Fragment.h"

#include <vector>

// かけらマネージャ
class FragmentManager {
public:
	void Initialize();

	void Update();

	void AddFragment(const Vector3& popTranslate);

private:

	// かけらのリスト
	std::vector<std::unique_ptr<Fragment>> fragments_;
};