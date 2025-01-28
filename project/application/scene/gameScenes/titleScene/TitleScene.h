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
#include "system/DamagePieceManager/DamagePieceManager.h"

class TitleScene :public BaseScene {
public:
	TitleScene() = default;
	~TitleScene() = default;
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
	// 地球
	std::unique_ptr<Earth> earth_ = nullptr;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// ダメージ破片
	std::unique_ptr<DamagePieceManager> damagePieceManager_ = nullptr;

};
