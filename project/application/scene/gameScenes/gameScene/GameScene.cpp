#include "GameScene.h"

#include "framework/SUGER.h"

void GameScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();
	// レベルデータをシーンにインポート
	levelDataImporter_.Import("baseScene");

}

void GameScene::Finalize() {

}

void GameScene::SceneStatePlayInitialize() {

}

void GameScene::SceneStatePlayUpdate() {

	//
	// コライダーの処理ここから
	//

	// コライダーコンテナをリセット
	SUGER::ClearColliderContainer();

}
