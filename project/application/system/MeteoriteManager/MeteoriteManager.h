#pragma once

#include "objects/Meteorite/Meteorite.h"

#include <vector>

#include <list>

// 前方宣言
class Moon;
class Player;
class FragmentManager;
class DamagePieceManager;

enum class MeteoritePopPlace {
	LeftTop,
	LeftBottom,
	RightBottom,
	RightTop,
};

// 隕石マネージャ
class MeteoriteManager {
public:
	void Initialize(Moon* earth, Player* player, FragmentManager* fragmentManager, DamagePieceManager* damagePieceManager);

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
	int32_t popNum_ = 1;

	int32_t popTimer_ = 0;

	int32_t popIntervalTime_ = 900;

	uint32_t currentSerialNumber_ = 0;

	int count = 0;

private:
	// 地球クラスのインスタンスを受け取る
	Moon* moon_ = nullptr;
	//
	Player* player_ = nullptr;
	// かけらマネージャのインスタンスを受け取る
	FragmentManager* fragmentManager_ = nullptr;
	DamagePieceManager* damagePieceManager_ = nullptr;
};