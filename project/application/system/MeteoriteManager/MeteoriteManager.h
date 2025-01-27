#pragma once

#include "objects/Meteorite/Meteorite.h"

#include <vector>

#include <list>

// 前方宣言
class Earth;
class Player;
class FragmentManager;

enum class MeteoritePopPlace {
	LeftTop,
	LeftBottom,
	RightBottom,
	RightTop,
};

// 隕石マネージャ
class MeteoriteManager {
public:
	void Initialize(Earth* earth, Player* player, FragmentManager* fragmentManager);

	void Update();

	void AddMeteorite(const Vector3& popTranslate);

	void AddColliderList();

	void PopMateorites();

private:

	// 
	// 調整パラメータ
	// 

	// データグループ名の定数
	const std::string kParamaterString_ = "MeteoriteParamater";

	float speed_ = 0.0f;

	// 隕石リスト
	std::list<std::unique_ptr<Meteorite>> meteorites_;

	// 沸く場所の列挙型
	MeteoritePopPlace popPlace_ = MeteoritePopPlace::RightBottom;

	// 沸く場所
	std::array<Vector3, 4> popPosition_;

	// 一度に沸く数
	int32_t popNum_ = 4;

	int32_t popTimer_ = 0;

	int32_t popIntervalTime_ = 300;

	uint32_t currentSerialNumber_ = 0;

	int count = 0;

private:
	// 地球クラスのインスタンスを受け取る
	Earth* earth_ = nullptr;
	//
	Player* player_ = nullptr;
	// かけらマネージャのインスタンスを受け取る
	FragmentManager* fragmentManager_ = nullptr;
};