// This
#include "SceneFactory.h"

#include "scene/gameScenes/sample/SampleScene.h"
#include "scene/gameScenes/load/LoadScene.h"
#include "scene/gameScenes/gameScene/GameScene.h"
#include "scene/gameScenes/titleScene/TitleScene.h"
#include "scene/gameScenes/resultScene/ResultScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "SAMPLE") {
		newScene = new SampleScene();
	} else if (sceneName == "LOAD") {
		newScene = new LoadScene();
	} else if (sceneName == "GAME") {
		newScene = new GameScene();
	} else if (sceneName == "TITLE") {
		newScene = new TitleScene();
	} else if (sceneName == "RESULT") {
		newScene = new ResultScene();
	}

	return newScene;
}
