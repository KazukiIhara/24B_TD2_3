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
	std::unique_ptr<Moon> moon_ = nullptr;

	float inclination_ = 23.4f;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// ダメージ破片
	std::unique_ptr<DamagePieceManager> damagePieceManager_ = nullptr;

	// title
	std::unique_ptr<Object2DController> titleText_;
	Vector2 titleTextPosition_ = { 0.0f,0.0f };

	// Aボタン
	std::unique_ptr<Object2DController> startUI_;
	Vector2 A_UIPosition_ = { 0.0f,0.0f };
	Vector2 A_UISize_ = { 0.0f,0.0f };

	// titleani
	std::unique_ptr<Object2DController> titleTextAni_;

	// Aボタン
	std::unique_ptr<Object2DController> F11UI_;



	int32_t aniTimer_ = 0;
	int32_t aniTime_ = 110;
	Vector2 constAniSize_{};
	Vector2 anisize_{};
	float aniAlpha_ = 1.0f;

	float time_ = 0;
	int clock_ = 1;
};
