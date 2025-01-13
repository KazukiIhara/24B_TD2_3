#pragma once

#include "objects/Meteorite/Meteorite.h"

#include <vector>

class Earth;

class MeteoriteManager {
public:
	void Initialize();

	void Update();

	void AddMeteorite();
private:
	// 隕石リスト
	std::vector<std::unique_ptr<Meteorite>> meteorites_;
};