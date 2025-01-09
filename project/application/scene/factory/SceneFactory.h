#pragma once

// MyHedder
#include "iScene/abstractFactory/AbstractSceneFactory.h"

// 今回のゲーム用のシーン作成クラス
class SceneFactory :public AbstractSceneFactory {
public:
	// シーン生成
	BaseScene* CreateScene(const std::string& sceneName)override;
};