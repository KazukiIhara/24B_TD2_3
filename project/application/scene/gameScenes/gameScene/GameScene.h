#pragma once

// Common
#include "iScene/base/BaseScene.h"
#include "2d/controller/Object2dController.h"
#include "3d/entityController/EntityController.h"
#include "VFX/particle/emitterController/EmitterController.h"
#include "3d/lineController/LineController.h"

// GameScene
#include "objects/player/Player.h"
#include "objects/Earth/Earth.h"
#include "objects/Skydome/Skydome.h"

#include "system/MeteoriteManager/MeteoriteManager.h"
#include "system/FragmentManager/FragmentManager.h"
#include "system/BumpManager/BumpManager.h"

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

	//
	// forDebug
	//

	std::unique_ptr<EntityController> plane_ = nullptr;

	//
	// Object
	//

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 地球
	std::unique_ptr<Earth> earth_ = nullptr;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 隕石マネージャ
	std::unique_ptr<MeteoriteManager> meteoriteManager_ = nullptr;
	// かけらマネージャ
	std::unique_ptr<FragmentManager> fragmentManager_ = nullptr;
	// たんこぶマネージャー
	std::unique_ptr<BumpManager> bumpManager_ = nullptr;

	//
	// スプライト
	//

	// 地球のHPUI
	std::array<std::unique_ptr<Object2DController>, 4> earthHPUI_;
	Vector2 earthUIPosition_ = { 0.0f,0.0f };

	std::array<std::unique_ptr<Object2DController>, 3> earthHpNumUI_;
	std::array<int32_t, 3> earthHpNum_;
	Vector2 earthHpNumUIPosition_ = { 0.0f,0.0f };
	Vector2 numberTextureSize_ = { 128.0f,192.0f };
	float numGap_ = 64.0f;

};

