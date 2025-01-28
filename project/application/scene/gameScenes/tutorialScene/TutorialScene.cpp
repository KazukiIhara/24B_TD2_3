#include "TutorialScene.h"

#include "framework/SUGER.h"

void TutorialScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	//
	// ムーン少佐スプライト作成
	//

	// 操作テキスト
	for (uint32_t i = 0; i < static_cast<uint32_t>(MMTutorial::num); i++) {
		moonMajarTurorials_[i] = std::make_unique<Object2DController>();
	}
	// オブジェクト作成
	moonMajarTurorials_[0]->Initialize(SUGER::Create2DObject("0_MM_Operation", "MoonMajorText/OperationTutorialText_x1025y192.png"));
	moonMajarTurorials_[1]->Initialize(SUGER::Create2DObject("0_MM_Fragment", "MoonMajorText/FragmentTutorialText_x1025y192.png"));
	moonMajarTurorials_[2]->Initialize(SUGER::Create2DObject("0_MM_Bump", "MoonMajorText/BumpTutorialText_x1025y192.png"));
	// 各種設定
	for (uint32_t i = 0; i < static_cast<uint32_t>(MMTutorial::num); i++) {
		tutorialPages_[i] = static_cast<uint32_t>(moonMajarTurorials_[i]->GetSize().y / moonMajarTextCutSize_.y);
		moonMajarTurorials_[i]->SetCutOutSize(moonMajarTextCutSize_);
		moonMajarTurorials_[i]->SetSize(moonMajarTextCutSize_);
		moonMajarTurorials_[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
		moonMajarTurorials_[i]->SetPosition(moonMajarTextPosition_);
		moonMajarTurorials_[i]->SetIsActive(false);
	}

	moonMajarTurorials_[0]->SetIsActive(true);

}

void TutorialScene::Finalize() {

}

void TutorialScene::SceneStatePlayInitialize() {

}

void TutorialScene::SceneStatePlayUpdate() {
	switch (currentTutorial_) {
	case MMTutorial::Operation:
		moonMajarTurorials_[0]->SetIsActive(true);
		moonMajarTurorials_[1]->SetIsActive(false);
		moonMajarTurorials_[2]->SetIsActive(false);
		break;
	case MMTutorial::Fragment:
		moonMajarTurorials_[0]->SetIsActive(false);
		moonMajarTurorials_[1]->SetIsActive(true);
		moonMajarTurorials_[2]->SetIsActive(false);
		break;
	case MMTutorial::Bump:
		moonMajarTurorials_[0]->SetIsActive(false);
		moonMajarTurorials_[1]->SetIsActive(false);
		moonMajarTurorials_[2]->SetIsActive(true);
		break;
	}

	// ムーン少佐ページ管理
	for (uint32_t i = 0; i < static_cast<uint32_t>(MMTutorial::num); i++) {
		moonMajarTurorials_[i]->SetLeftTop(Vector2(0.0f, static_cast<float>(currentPage_) * moonMajarTextCutSize_.y));
	}

	// ボタンでページ送り
	if (SUGER::TriggerButton(0, ButtonA) || SUGER::TriggerKey(DIK_SPACE)) {

		currentPage_++;
		if (currentPage_ == tutorialPages_[static_cast<uint32_t>(currentTutorial_)]) {
			currentPage_ = 0;
			uint32_t current = static_cast<uint32_t>(currentTutorial_);
			current++;
			currentTutorial_ = static_cast<MMTutorial>(current);
		}
	}

	if (static_cast<uint32_t>(currentTutorial_) == 3) {
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
