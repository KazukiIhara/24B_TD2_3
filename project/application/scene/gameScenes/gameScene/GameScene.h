#pragma once

// Common
#include "iScene/base/BaseScene.h"
#include "2d/controller/Object2dController.h"
#include "3d/entityController/EntityController.h"
#include "VFX/particle/emitterController/EmitterController.h"
#include "3d/lineController/LineController.h"

// GameScene
#include "objects/player/Player.h"
#include "objects/Earth/Moon.h"
#include "objects/Skydome/Skydome.h"

#include "system/MeteoriteManager/MeteoriteManager.h"
#include "system/FragmentManager/FragmentManager.h"
#include "system/BumpManager/BumpManager.h"
#include "system/DamagePieceManager/DamagePieceManager.h"
#include "system/UFOManager/UFOManager.h"
#include "system/UFOBulletManager/UFOBulletManager.h"


class GameScene :public BaseScene {
public:
	GameScene() = default;
	~GameScene() = default;
	// 初期化
	void Initialize()override;
	// 終了
	void Finalize()override;

	// プレイフェーズ初期化
	void SceneStatePlayInitialize()override;
	// プレイフェーズ更新
	void SceneStatePlayUpdate()override;
	// フェードアウト終了時
	void SceneStateFadeOutUpdate()override;

	std::array<int32_t, 3> SplitDigits(int32_t number);

private:
	// 日数加算用タイマー
	float scoreTimer_ = 0.0f;

	//
	// forDebug
	//

	std::unique_ptr<EntityController> plane_ = nullptr;

	//
	// Object
	//

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 月
	std::unique_ptr<Moon> moon_ = nullptr;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 隕石マネージャ
	std::unique_ptr<MeteoriteManager> meteoriteManager_ = nullptr;
	// かけらマネージャ
	std::unique_ptr<FragmentManager> fragmentManager_ = nullptr;
	// たんこぶマネージャー
	std::unique_ptr<BumpManager> bumpManager_ = nullptr;
	// UFOマネージャ
	std::unique_ptr<UFOManager> ufoManager_ = nullptr;
	// UFO弾マネージャ
	std::unique_ptr<UFOBulletManager> ufoBulletManager_ = nullptr;

	// ダメージ破片
	std::unique_ptr<DamagePieceManager> damagePieceManager_ = nullptr;

	//
	// スプライト
	//

	// ムーン少佐
	std::unique_ptr<Object2DController> moonMajarSprite_ = nullptr;
	Vector2 moonMajarPosition_ = { 960.0f,951.0f };
	float moonMajarAlpha_ = 1.0f;

	// 地球のHPUI
	std::array<std::unique_ptr<Object2DController>, 4> earthHPUI_;
	Vector2 earthUIPosition_ = { 0.0f,0.0f };
	Vector2 earthUISize_ = { 0.0f,0.0f };

	std::array<std::unique_ptr<Object2DController>, 3> earthHpNumUI_;
	std::array<int32_t, 3> earthHpNum_;
	Vector2 earthHpNumUIPosition_ = { 0.0f,0.0f };
	Vector2 numberTextureSize_ = { 128.0f,192.0f };
	float numGap_ = 64.0f;


	// 経過日数UI
	std::array<std::unique_ptr<Object2DController>, 3> currentDaysNumUI_;
	std::array<int32_t, 3> currentDaysNum_;
	Vector2 currentDaysPosition_{ 0.0f,0.0f };

	// 経過年数UI
	std::array<std::unique_ptr<Object2DController>, 3> currentYearsNumUI_;
	std::array<int32_t, 3> currentYearsNum_;
	Vector2 currentYearsPosition_{ 0.0f,0.0f };

	// シンボル
	std::array<std::unique_ptr<Object2DController>, 2> symbolUI_;
	Vector2 yearPosition_ = { 0.0f,0.0f };
	Vector2	daysPosition_ = { 0.0f,0.0f };

	// 経過日数
	uint32_t currentDays_ = 0;
	// 経過年数
	uint32_t currentYears_ = 0;
};