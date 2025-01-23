#pragma once

#include "objects/Fragment/Fragment.h"

#include <vector>
#include <list>

class Earth;
class Player;

enum PopPlace {
	Top,
	LeftTop,
	Left,
	LeftBottom,
	Bottom,
	RightBottom,
	Right,
	RightTop,
};

// かけらマネージャ
class FragmentManager {
public:
	void Initialize();

	void Update();

	void AddFragment(const Vector3& popTranslate);

	void AddColliderList();

	void SetEarth(Earth* earth);
	void SetPlayer(Player* player);

	void PopFragments();

	void SwitchPopPosition();

private:
	// 沸く場所の種類
	PopPlace popPlace_ = PopPlace::Top;

	// 沸く場所
	std::array<Vector3, 8> popPosition_;

	// 一度に沸く数
	int32_t popNum_ = 4;

	// かけらのリスト
	std::list<std::unique_ptr<Fragment>> fragments_;

	int32_t popTimer_ = 0;

	int32_t popIntervalTime_ = 300;

	uint32_t currentSerialNumber_ = 0;

private:
	Earth* earth_ = nullptr;
	Player* player_ = nullptr;

};