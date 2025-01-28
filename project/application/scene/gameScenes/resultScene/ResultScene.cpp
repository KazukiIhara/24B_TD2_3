#include "ResultScene.h"

#include "framework/SUGER.h"

void ResultScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();


}

void ResultScene::Finalize() {}

void ResultScene::SceneStatePlayInitialize() {}

void ResultScene::SceneStatePlayUpdate() {

	ImGui::Begin("Result");

	ImGui::Text("%u Years %u Days", GetGameData().years_, GetGameData().days_);

	ImGui::End();


	if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
		ChangeScene("TITLE");
	}

}
