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

enum class MMTutorial {
	Operation,
	Fragment,
	Bump,

	num,
};

class TutorialScene:public BaseScene {
public:
	TutorialScene() = default;
	~TutorialScene() = default;

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

private:

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 月
	std::unique_ptr<Moon> moon_ = nullptr;

	float inclination_ = 23.4f;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// ダメージ破片
	std::unique_ptr<DamagePieceManager> damagePieceManager_ = nullptr;

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


	//
	// ムーン少佐スプライト
	//

	// チュートリアル
	std::unique_ptr<Object2DController> moonMajarTurorial_;
	// 少佐のテキストの座標
	Vector2 moonMajarTextPosition_ = { 960.0f,951.0f };
	// 少佐のテキストの切り取りサイズ
	const Vector2 moonMajarTextCutSize_ = { 1025.0f,192.0f };
	// チュートリアルごとのページ数
	uint32_t tutorialPages_ = 15;
	// 現在のページ
	uint32_t currentPage_ = 0;

	std::array<std::unique_ptr<Object2DController>, 5> tutorialTexs_;

};