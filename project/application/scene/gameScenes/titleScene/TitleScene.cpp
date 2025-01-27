#include "TitleScene.h"

#include "framework/SUGER.h"

void TitleScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

}

void TitleScene::Finalize() {

}

void TitleScene::SceneStatePlayInitialize() {

}

void TitleScene::SceneStatePlayUpdate() {
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	if (ImGui::Button("ChangeScene")) {
		ChangeScene("GAME");
	}
	ImGui::End();
#endif // _DEBUG

}
