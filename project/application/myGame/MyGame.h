#pragma once
// MyHedder
#include "framework/SUGER.h"
#include "scene/factory/SceneFactory.h"
#include "manager/scene/SceneManager.h"

// ゲームクラス
class MyGame :public SUGER {
public: // 公開メンバ関数
	MyGame() = default;
	~MyGame();

	// 初期化
	void Initialize()override;
	// 終了
	void Finalize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw()override;

private: // メンバ変数
	// シーンファクトリ
	std::unique_ptr<SceneFactory> sceneFactory_ = nullptr;
	// シーンマネージャ
	std::unique_ptr<SceneManager> sceneManager_ = nullptr;
};
