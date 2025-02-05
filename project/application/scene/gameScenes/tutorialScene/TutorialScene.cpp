#include "TutorialScene.h"

#include "framework/SUGER.h"

void TutorialScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	//
	// ムーン少佐スプライト作成
	//
	moonMajarTurorial_ = std::make_unique<Object2DController>();
	// オブジェクト作成
	moonMajarTurorial_->Initialize(SUGER::Create2DObject("0_MM_Operation", "MoonMajorText/TutorialText_x1025y192.png"));
	// 各種設定
	moonMajarTurorial_->SetCutOutSize(moonMajarTextCutSize_);
	moonMajarTurorial_->SetSize(moonMajarTextCutSize_);
	moonMajarTurorial_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	moonMajarTurorial_->SetPosition(moonMajarTextPosition_);
	moonMajarTurorial_->SetIsActive(false);

	moonMajarTurorial_->SetIsActive(true);

}

void TutorialScene::Finalize() {

}

void TutorialScene::SceneStatePlayInitialize() {

}

void TutorialScene::SceneStatePlayUpdate() {

	moonMajarTurorial_->SetLeftTop(Vector2(0.0f, static_cast<float>(currentPage_) * moonMajarTextCutSize_.y));
	// ボタンでページ送り
	if (SUGER::TriggerButton(0, ButtonA) || SUGER::TriggerKey(DIK_SPACE)) {
		currentPage_++;
	}

	if (currentPage_ == 15) {
		sceneStateRequest_ = SceneState::kFadeOut;
	}

}

void TutorialScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		ChangeScene("GAME");
		fade_->Stop();
	}
}
