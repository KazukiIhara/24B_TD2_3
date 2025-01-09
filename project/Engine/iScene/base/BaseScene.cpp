#include "BaseScene.h"

#include "framework/SUGER.h"
#include "imgui.h"
#include "debugTools/logger/Logger.h"
#include "manager/scene/SceneManager.h"

void BaseScene::Initialize() {
	// 各コンテナのクリア
	// Collider
	SUGER::ClearColliderContainer();
	// Line
	SUGER::ClearLineContainer();
	// Emitter
	SUGER::ClearEmitterContainer();
	// Particle
	SUGER::ClearParticleContainer();
	// Entity
	SUGER::ClearEntityContainer();
	// Empty
	SUGER::ClearEmptyContainer();
	// 2DObject
	SUGER::Clear2DObjectContainer();
	// デバッグログ
	Logger::Log("AllObjetcsCleared\n");

	// デバッグカメラ作成
	debugCamera_ = std::make_unique<Camera>();
	debugCamera_->Initialize();

	// シーンカメラ作成
	sceneCamera_ = std::make_unique<Camera>();
	sceneCamera_->Initialize();

	// ライト作成
	light_ = std::make_unique<PunctualLight>();
	light_->Initialize();

	// フェード作成
	fade_ = std::make_unique<Fade>();
	// フェード初期化
	fade_->Initialize();
	// フェードインリクエスト
	sceneStateRequest_ = SceneState::kFadeIn;

	// シーンに必要なカメラとライトのセット
	SUGER::SetRequiredObjects(sceneCamera_.get(), light_.get());

	// コライダーデバッグ描画用のライングループを作成
	SUGER::InitializeColliderLineGroup();

	// FixFPSを初期化
	SUGER::InitializeFixFPS();
}

void BaseScene::Update() {
	// デバッグカメラコントロール
	DebugCameraOperation();
	// デバッグカメラのアップデート
	debugCamera_->Update();
	// シーンカメラの更新
	sceneCamera_->Update();
	// ライトの更新
	light_->Update();
	// シーンステータスのリクエスト初期化処理
	SceneStatusInitizlize();
	// シーンステータスの更新処理
	SceneStatusUpdate();
}

void BaseScene::ChangeScene(const std::string& nextScene) {
	sceneManager_->ChangeScene(nextScene);
}

void BaseScene::SetSceneManager(SceneManager* sceneManager) {
	sceneManager_ = sceneManager;
}

void BaseScene::DebugCameraOperation() {
#ifdef _DEBUG
	// 回転と移動量を持ってくる
	Vector3 cameraRotate = debugCamera_->GetRotate();
	Vector3 cameraTranslate = debugCamera_->GetTranslate();

	// キーボード入力でカメラフラグ切り替え
	if (SUGER::TriggerKey(DIK_P)) {
		if (isActiveDebugCamera_) {
			isActiveDebugCamera_ = false;
			SUGER::SetSceneCamera(sceneCamera_.get());
		} else {
			isActiveDebugCamera_ = true;
			SUGER::SetSceneCamera(debugCamera_.get());
		}
	}

	// デバッグカメラが有効の場合
	if (isActiveDebugCamera_) {

		// マウス入力の取得
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		// ウィンドウ座標系へ変換
		ScreenToClient(SUGER::GetWindowHandle(), &cursorPos);

		static POINT lastCursorPos = cursorPos;
		POINT delta;
		delta.x = cursorPos.x - lastCursorPos.x;
		delta.y = cursorPos.y - lastCursorPos.y;

		// マウスホイールの値を取得してズーム操作
		int64_t wheelDelta = SUGER::GetMouseWheelDelta();

		// カメラ回転処理
		HandleCameraRotation(cameraRotate, delta);
		// カメラ移動処理
		HandleCameraTranslation(cameraTranslate, cameraRotate, delta);
		// カメラズーム処理
		HandleCameraZoom(cameraTranslate, cameraRotate, wheelDelta);

		// 更新された値をカメラに反映
		debugCamera_->SetRotate(cameraRotate);
		debugCamera_->SetTranslate(cameraTranslate);

		// 現在のカーソル位置を保存
		lastCursorPos = cursorPos;
	}

	// ImGuiによるカメラ操作
	DebugCameraUI(cameraRotate, cameraTranslate);

	// 回転と移動量を返す
	debugCamera_->SetRotate(cameraRotate);
	debugCamera_->SetTranslate(cameraTranslate);

#endif // _DEBUG
}

void BaseScene::DebugCameraUI(Vector3& cameraRotate, Vector3& cameraTranslate) {
	// ImGuiの処理
	ImGui::Begin("DebugCamera");
	ImGui::Text("Push P Toggle Camera");
	// デバッグカメラが有効なら
	if (isActiveDebugCamera_) {
		ImGui::Text("State: Enable");
		if (ImGui::Button("DisableDebugCamera")) {
			SUGER::SetSceneCamera(sceneCamera_.get());
			isActiveDebugCamera_ = false;
		}
		ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &cameraTranslate.x, 0.01f);
	} else {
		ImGui::Text("State: Disable");
		if (ImGui::Button("EnableDebugCamera")) {
			SUGER::SetSceneCamera(debugCamera_.get());
			isActiveDebugCamera_ = true;
		}
	}
	ImGui::End();
}

void BaseScene::HandleCameraRotation(Vector3& cameraRotate, const POINT& delta) {
	// マウスの右ボタンが押されているか確認
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		// カメラの回転を更新
		cameraRotate.x -= delta.y * 0.001f; // 縦方向
		cameraRotate.y -= delta.x * 0.001f; // 横方向
	}
}

void BaseScene::HandleCameraTranslation(Vector3& cameraTranslate, Vector3& cameraRotate, const POINT& delta) {
	// 中ボタンドラッグで移動
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		// 回転からカメラの右方向ベクトルを計算
		Vector3 right;
		right.x = std::cosf(cameraRotate.y);
		right.y = 0.0f;
		right.z = -std::sinf(cameraRotate.y);

		// 回転からカメラの上方向ベクトルを計算
		Vector3 up;
		up.x = std::sinf(cameraRotate.x) * std::sinf(cameraRotate.y);
		up.y = std::cosf(cameraRotate.x);
		up.z = std::sinf(cameraRotate.x) * std::cosf(cameraRotate.y);

		// 移動量をローカル座標系で計算
		const float moveSpeed = 0.002f;
		Vector3 moveDelta = (right * static_cast<float> (-delta.x) + up * static_cast<float> (delta.y)) * moveSpeed;

		// カメラ位置を更新
		cameraTranslate += moveDelta;
	}
}

void BaseScene::HandleCameraZoom(Vector3& cameraTranslate, Vector3& cameraRotate, int64_t wheelDelta) {
	if (wheelDelta != 0) {
		const float zoomSpeed = 0.001f; // ズーム速度スケール

		// カメラの forward ベクトルを取得
		Vector3 forward = Forward(cameraRotate);

		// forward に沿ってカメラの位置を更新
		cameraTranslate += forward * (wheelDelta * zoomSpeed);
	}
}

void BaseScene::SceneStatusInitizlize() {
	// シーンの状態
	if (sceneStateRequest_) {
		// 振る舞いを変更する
		sceneState_ = sceneStateRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (sceneState_) {
			case SceneState::kFadeIn:
				SceneStateFadeInInitialize();
				break;
			case SceneState::kPlay:
				SceneStatePlayInitialize();
				break;
			case SceneState::kFadeOut:
				SceneStateFadeOutInitialize();
				break;
		}
		// 振る舞いリクエストをリセット
		sceneStateRequest_ = std::nullopt;
	}
}

void BaseScene::SceneStatusUpdate() {
	switch (sceneState_) {
		case kFadeIn:
			SceneStateFadeInUpdate();
			break;
		case kPlay:
			SceneStatePlayUpdate();
			break;
		case kFadeOut:
			SceneStateFadeOutUpdate();
			break;
	}
}

void BaseScene::SceneStateFadeInInitialize() {
	// フェード開始
	fade_->Start(FadeStatus::FadeIn, 0.5f);
}

void BaseScene::SceneStateFadeInUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		sceneStateRequest_ = SceneState::kPlay;
		fade_->Stop();
	}
}

void BaseScene::SceneStateFadeOutInitialize() {
	// フェード開始
	fade_->Start(FadeStatus::FadeOut, 0.5f);
}

void BaseScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		fade_->Stop();
		// ここに遷移先を指定したシーンチェンジ関数を呼び出す

	}
}
