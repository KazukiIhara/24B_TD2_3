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
#include "system/DamagePieceManager/DamagePieceManager.h"

enum class MMTutorial {
	Operation,
	Fragment,
	Bump,

	num,
};

class TutorialScene :public BaseScene {
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
	//
	// ムーン少佐スプライト
	//

	// チュートリアル
	std::array<std::unique_ptr<Object2DController>, 3> moonMajarTurorials_;
	// 少佐のテキストの座標
	Vector2 moonMajarTextPosition_ = { 960.0f,951.0f };
	// 少佐のテキストの切り取りサイズ
	const Vector2 moonMajarTextCutSize_ = { 1025.0f,192.0f };

};