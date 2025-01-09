// This
#include "SceneManager.h"

// C++
#include <cassert>

// MyHedder
#include "iScene/base/BaseScene.h"

SceneManager::~SceneManager() {
	// 最後のシーンの終了と解放
	currentScene_->Finalize();
	delete currentScene_;
}

void SceneManager::Update() {
	// シーン切り替え処理
	SwitchScene();
	// 現在のシーンの更新処理
	currentScene_->Update();
}

void SceneManager::SwitchScene() {
	// 次シーンの予約があるなら
	if (nextScene_) {
		// 旧シーン終了
		if (currentScene_) {
			currentScene_->Finalize();
			delete currentScene_;
		}

		// シーン切り替え
		currentScene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャをセット
		currentScene_->SetSceneManager(this);

		// 次のシーンを初期化
		currentScene_->Initialize();
	}
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次シーンを作成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}