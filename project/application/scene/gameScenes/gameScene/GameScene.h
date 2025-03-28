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
#include "objects/Boss/Boss.h"

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

	void InitializeUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize);
	std::array<int32_t, 5> SplitDigits5(int32_t number);
	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<int32_t, 5>& num, Vector2 position, int number, float numGap);


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

	// ボス
	std::unique_ptr<Boss> boss_ = nullptr;
	Vector3 bossPopPosition_ = { 45.0f,0.0f,0.0f };
	Vector3 bossBattleBeginPosition_ = { 25.0f,0.0f,0.0f };

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

	// Warningスプライト
	std::unique_ptr<Object2DController> warningSprite_ = nullptr;
	// Warningスプライト2
	std::unique_ptr<Object2DController> warningSprite2_ = nullptr;

	// Warningテキスト
	std::unique_ptr<Object2DController> warningTex_ = nullptr;
	std::unique_ptr<Object2DController> warningTex2_ = nullptr;

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
	float numGap2_ = 48.0f;


	// 経過日数UI
	std::array<std::unique_ptr<Object2DController>, 3> currentDaysNumUI_;
	std::array<int32_t, 3> currentDaysNum_;
	Vector2 currentDaysPosition_{ 0.0f,0.0f };

	// 経過年数UI
	std::array<std::unique_ptr<Object2DController>, 3> currentBossBattleNumUI_;
	std::array<int32_t, 3> currentBossBattleNum_;
	Vector2 currentYearsPosition_{ 0.0f,0.0f };

	// シンボル
	std::array<std::unique_ptr<Object2DController>, 2> symbolUI_;
	Vector2 yearPosition_ = { 0.0f,0.0f };
	Vector2	daysPosition_ = { 0.0f,0.0f };

	// パーセントUI 
	std::unique_ptr<Object2DController> parsentUI_ = nullptr;
	Vector2 parsentPosition_ = { 1730.0f,850.0f };

	// シンボル
	std::unique_ptr<Object2DController> buttomUI_;
	Vector2 buttomUIPosition_ = { 512.0f / 2 ,990.0f };
	Vector2 buttomUiSize_ = { 512,96 * 2 };

	// フェード用テクスチャ
	std::unique_ptr<Object2DController> whiteFade_;
	float alpha_ = 0.0f;

	// 経過日数
	uint32_t currentDays_ = 0;
	// 経過年数
	uint32_t currentYears_ = 0;

	bool isBossFightStart_ = false;
	float bossFightStartTimer_ = 0.0f;
	float bossFightStartTime_ = 300.0f;

	bool isBossFight_ = false;

	uint32_t bossBattleframeCount_ = 0;
	uint32_t bossBattleTimer_ = 120;

	bool gameOverAnimation_ = false;
	float gameOverAnimationTime_ = 180.0f;
	float gameOverAnimationTimer_ = 0.0f;


	float playerBangTime_ = 180.0f;
	float playerBangTimer_ = 0.0f;


	//
	Vector2 numberTextureSize2_ = { 48.0f,96.0f };
	std::array<std::unique_ptr<Object2DController>, 5> scoreUI_;
	std::array<int32_t, 5> scoreNum_;
	Vector2 scorePosition_{ 1700 - 32,30 + 32 };
	uint32_t score_ = 0;

	//
	// スコア(テキスト)
	std::unique_ptr<Object2DController> resultScore_;
	Vector2 resultScorePos_{ 1820 - 32,30 + 32 };
	Vector2 scoreSize{ 256,96 };
};