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
		sceneStateRequest_ = SceneState::kFadeOut;
	}
	ImGui::End();
#endif // _DEBUG

	if (SUGER::TriggerKey(DIK_SPACE)) {
		sceneStateRequest_ = SceneState::kFadeOut;
	}
}

void TitleScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		ChangeScene("GAME");
		fade_->Stop();
	}
}
