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

}

void TutorialScene::SceneStateFadeOutUpdate() {

}
