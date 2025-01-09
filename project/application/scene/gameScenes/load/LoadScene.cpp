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
		ChangeScene("SAMPLE");
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

	// テクスチャ読み込み
	// テクスチャの規定ディレクトリパス
	const std::string textureDirectoryPath = "resources/images/";

	SUGER::LoadTexture(textureDirectoryPath + "myUvChecker.png");
	SUGER::LoadTexture(textureDirectoryPath + "circle.png");
	SUGER::LoadTexture(textureDirectoryPath + "pronama_chan.png");

	// モデル読み込み
	SUGER::LoadModel("walk");
	SUGER::LoadModel("pronama_chan");
	SUGER::LoadModel("teapot");

	// レベルデータ読み込み
	SUGER::LoadJsonLevelData("baseScene");

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

	// ボタンでフェードアウト処理
	if (SUGER::TriggerKey(DIK_SPACE)) {
		sceneStateRequest_ = SceneState::kFadeOut;
	}
}
