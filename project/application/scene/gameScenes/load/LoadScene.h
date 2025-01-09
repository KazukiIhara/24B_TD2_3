#pragma once

// Common
#include "iScene/base/BaseScene.h"
#include "2d/controller/Object2dController.h"
#include "3d/entityController/EntityController.h"
#include "VFX/particle/emitterController/EmitterController.h"

class LoadScene :public BaseScene {
public:
	enum LoadSceneState {
		kPreLoad,
		kLoading,
		kFinished,
	};
	LoadScene() = default;
	~LoadScene() = default;

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

	// ロードシーン状態初期化
	void LoadSceneStateInitialize();
	// ロードシーン状態更新
	void LoadSceneStateUpdate();

	// ロード前状態初期化
	void LoadSceneStatePreLoadInitialize();
	// ロード前状態更新
	void LoadSceneStatePreLoadUpdate();

	// ロード中状態初期化
	void LoadSceneStateLoadingInitialize();
	// ロード中状態更新
	void LoadSceneStateLoadingUpdate();

	// ロード終了状態初期化
	void LoadSceneStateFinishedInitizlize();
	// ロード終了状態更新
	void LoadSceneStateFinishedUpdate();

private:
	// ロードシーンのフェーズ
	LoadSceneState loadSceneState_ = LoadSceneState::kPreLoad;
	// 次のロードシーンのフェーズ
	std::optional<LoadSceneState> loadSceneStateRequest_ = std::nullopt;

};