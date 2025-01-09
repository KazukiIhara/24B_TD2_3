#pragma once

// c++
#include <string>

// MyHedder
#include "iScene/abstractFactory/AbstractSceneFactory.h"

// 前方宣言
class BaseScene;

// シーン管理クラス
class SceneManager {
public: // メンバ関数
	SceneManager() = default;
	~SceneManager();

	// 更新
	void Update();

	// シーン変更処理
	void SwitchScene();

	// 次のシーンを指定
	void ChangeScene(const std::string& sceneName);

	// シーンファクトリのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) {
		sceneFactory_ = sceneFactory;
	};

private:
	// 現在のシーン
	BaseScene* currentScene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリ(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};