// This
#include "SceneFactory.h"

#include "scene/gameScenes/sample/SampleScene.h"
#include "scene/gameScenes/load/LoadScene.h"
#include "scene/gameScenes/gameScene/GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "SAMPLE") {
		newScene = new SampleScene();
	} else if (sceneName == "LOAD") {
		newScene = new LoadScene();
	} else if (sceneName == "GAME") {
		newScene = new GameScene();
	}

	return newScene;
}
