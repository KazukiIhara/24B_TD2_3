#include "ResultScene.h"

#include "framework/SUGER.h"

void ResultScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();


}

void ResultScene::Finalize() {
}

void ResultScene::SceneStatePlayInitialize() {
}

void ResultScene::SceneStatePlayUpdate() {
	if (SUGER::TriggerKey(DIK_SPACE)) {
		ChangeScene("TITLE");
	}

}
