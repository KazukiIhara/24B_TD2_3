#pragma once

#include "3d/entityController/EntityController.h"

// たんこぶクラス
class Bump {
public:
	Bump() = default;
	~Bump() = default;

private:
	// 大きさ
	float scale_ = 1.0f;
};