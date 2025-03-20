#include "LoadScene.h"

#include "framework/SUGER.h"
#include "debugTools/logger/Logger.h"

void LoadScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// TODO:ロード画面用のテクスチャと2dオブジェクトの作成

	// TODO:ロード終了画面用のテクスチャと2dオブジェクトの作成

}

void LoadScene::Finalize() {
	Logger::Log("AllResources,Loaded");
}

void LoadScene::SceneStatePlayInitialize() {
	// ロード前状態のリクエストを送る
	loadSceneStateRequest_ = LoadSceneState::kPreLoad;
}

void LoadScene::SceneStatePlayUpdate() {
	// ロードシーンにリクエストに応じて初期化
	LoadSceneStateInitialize();
	// ロードシーン状態に応じて更新
	LoadSceneStateUpdate();
}

void LoadScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {

		//ChangeScene("TITLE");
		ChangeScene("RESULT");

		fade_->Stop();
	}
}

void LoadScene::LoadSceneStateInitialize() {
	// ロードシーンの状態
	if (loadSceneStateRequest_) {
		// 振る舞いを変更
		loadSceneState_ = loadSceneStateRequest_.value();
		// 状態ごとの初期化を実行
		switch (loadSceneState_) {
			case LoadScene::kPreLoad:
				LoadSceneStatePreLoadInitialize();
				break;
			case LoadScene::kLoading:
				LoadSceneStateLoadingInitialize();
				break;
			case LoadScene::kFinished:
				LoadSceneStateFinishedInitizlize();
				break;
		}
	}
}

void LoadScene::LoadSceneStateUpdate() {
	switch (loadSceneState_) {
		case LoadScene::kPreLoad:
			LoadSceneStatePreLoadUpdate();
			break;
		case LoadScene::kLoading:
			LoadSceneStateLoadingUpdate();
			break;
		case LoadScene::kFinished:
			LoadSceneStateFinishedUpdate();
			break;
	}
}

void LoadScene::LoadSceneStatePreLoadInitialize() {
	// ロード中のテクスチャをアクティブ

}

void LoadScene::LoadSceneStatePreLoadUpdate() {
	// ロード前に操作などあれば

	// 今の実装では操作がないので、即ロード状態のリクエストを送る
	loadSceneStateRequest_ = LoadSceneState::kLoading;
}

void LoadScene::LoadSceneStateLoadingInitialize() {
	// サウンド読み込み
	SUGER::LoadWaveSound("Alarm01.wav");
	SUGER::LoadWaveSound("BGM.wav");
	SUGER::LoadWaveSound("Title.wav");  //TitleSceneで流すBGM
	SUGER::LoadWaveSound("GameScene.wav");  //GameSceneで流すBGM
	SUGER::LoadWaveSound("BossBGM.wav");  //Bossと戦っている時に流すBGM
	SUGER::LoadWaveSound("Result.wav");  //ResultSceneで流すBGM

	SUGER::LoadWaveSound("Decision.wav");  //TitleやResultで使う決定音
	SUGER::LoadWaveSound("Selection.wav");  //少佐のテキストを進める時やリザルトの選択の時のSE

	SUGER::LoadWaveSound("EarthDanger.wav");  //地球のHPが25%以下になった時に流すSE
	SUGER::LoadWaveSound("EarthToFragmentDamage.wav"); //地球が欠片とぶつかった時のSE
	SUGER::LoadWaveSound("EarthHitDamage.wav");  //地球が隕石・UFOとぶつかった時のSE

	SUGER::LoadWaveSound("MoonDamage.wav");  //月が他のオブジェクト(欠片・隕石・UFO・ボス)にダメージを与えた時のSE

	SUGER::LoadWaveSound("MeteoriteBreak.wav");  //隕石・UFOが壊れた時のSE
	SUGER::LoadWaveSound("UFO_Attack.wav");  //UFOが攻撃する時のSE

	SUGER::LoadWaveSound("Warning.wav");  //ボスが登場する時のBGM
	SUGER::LoadWaveSound("Boss_Attack.wav");  //ボスが攻撃する時のSE



	// テクスチャ読み込み
	// テクスチャの規定ディレクトリパス
	const std::string textureDirectoryPath = "resources/images/";

	SUGER::LoadTexture(textureDirectoryPath + "myUvChecker.png");
	SUGER::LoadTexture(textureDirectoryPath + "circle.png");
	SUGER::LoadTexture(textureDirectoryPath + "pronama_chan.png");
	SUGER::LoadTexture(textureDirectoryPath + "Earth_HP_UI/Earth_HP_UI_25%.png");
	SUGER::LoadTexture(textureDirectoryPath + "Earth_HP_UI/Earth_HP_UI_50%.png");
	SUGER::LoadTexture(textureDirectoryPath + "Earth_HP_UI/Earth_HP_UI_75%.png");
	SUGER::LoadTexture(textureDirectoryPath + "Earth_HP_UI/Earth_HP_UI_100%.png");
	SUGER::LoadTexture(textureDirectoryPath + "TitleText/TitleText.png");
	SUGER::LoadTexture(textureDirectoryPath + "TutorialImages/TutorialImage_00.png");
	SUGER::LoadTexture(textureDirectoryPath + "TutorialImages/TutorialImage_01.png");
	SUGER::LoadTexture(textureDirectoryPath + "TutorialImages/TutorialImage_02.png");
	SUGER::LoadTexture(textureDirectoryPath + "TutorialImages/TutorialImage_03.png");
	SUGER::LoadTexture(textureDirectoryPath + "TutorialImages/TutorialImage_04.png");


	// モデル読み込み
	SUGER::LoadModel("teapot");
	SUGER::LoadModel("Moon");
	SUGER::LoadModel("Earth");
	SUGER::LoadModel("Meteorite");
	SUGER::LoadModel("Meteorite2");
	SUGER::LoadModel("Meteorite3");
	SUGER::LoadModel("Fragment");
	SUGER::LoadModel("Skydome");
	SUGER::LoadModel("Bump");
	SUGER::LoadModel("Plane");
	SUGER::LoadModel("DamagePiece");
	SUGER::LoadModel("DamagePiece2");
	SUGER::LoadModel("DamagePiece3");
	SUGER::LoadModel("DamagePiece4");
	SUGER::LoadModel("UFO");
	SUGER::LoadModel("UFO_Bullet");
	SUGER::LoadModel("Boss");

	// レベルデータ読み込み
	SUGER::LoadJsonLevelData("GameScene");

	// ロードによる待機時間を考慮して、FIXFPSの処理をリセットする
	SUGER::InitializeFixFPS();
}

void LoadScene::LoadSceneStateLoadingUpdate() {
	// ロード中に何か操作があれば

	// 今の実装だと、ここを通った時にロードが終了しているはずなので、即終了画面へ
	loadSceneStateRequest_ = LoadSceneState::kFinished;
}

void LoadScene::LoadSceneStateFinishedInitizlize() {
	// ロード終了画面をアクティブにする

}

void LoadScene::LoadSceneStateFinishedUpdate() {
#ifdef _DEBUG
	ImGui::Begin("LoadSceneState");
	ImGui::Text("AllResources,Loaded");
	ImGui::End();
#endif // DEBUG
	sceneStateRequest_ = SceneState::kFadeOut;

}
