#pragma once

#include "objects/UFO/UFO.h"

#include <list>

enum class UFOPopPlace {
	LeftTop,
	RightTop,
	LeftBottom,
	RightBottom,
};

class UFOManager {
public:

	void Initialize();

	void Update();

	void AddUFO(const Vector3& popTranslate, const Vector3 velocity);

	void AddColliderList();

	void PopUFOs();

private:
	// UFOリスト
	std::list<std::unique_ptr<UFO>> ufos_;

	// 沸く場所の列挙型
	UFOPopPlace popPlace_ = UFOPopPlace::LeftTop;

	// 沸く場所
	std::array<Vector3, 4> popPosition_;

	// 一度に沸く数
	int32_t popNum_ = 1;

	int32_t popTimer_ = 0;

	int32_t popIntervalTime_ = 900;

	uint32_t currentSerialNumber_ = 0;

	int count = 0;

};