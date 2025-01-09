#pragma once

// C++
#include <string>

// MyHedder
#include "iScene/base/BaseScene.h"

// シーン作成の抽象クラス
class AbstractSceneFactory {
public:// 仮想関数
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成関数
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};