#pragma once

#include "3d/empty/empty.h"

class EmptyController {
public:
	EmptyController() = default;
	~EmptyController() = default;

private:
	// コントロールするエンプティ
	Empty* empty_ = nullptr;
};