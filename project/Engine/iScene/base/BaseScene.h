#pragma once

// C++
#include <memory>
#include <optional>

// MyHedder
#include "3d/cameras/camera/Camera.h"
#include "3d/lights/punctualLight/PunctualLight.h"
#include "transition/fade/Fade.h"
#include "enum/SceneEnum.h"
#include "importer/level/json/JsonLevelDataImporter.h"

// 前方宣言
class SceneManager;

// シーンの基底クラス
class BaseScene {
public: // 仮想関数
	// 仮想デストラクタ
	virtual ~BaseScene() = default;
	// 初期化
	virtual void Initialize();
	// 終了
	virtual void Finalize() = 0;
	// 更新
	void Update();

	// シーンチェンジ
	void ChangeScene(const std::string& nextScene);

	// シーンマネージャのセット
	virtual void SetSceneManager(SceneManager* sceneManager);

protected:
	// ベースシーンの状態初期化処理
	void SceneStatusInitizlize();
	// ベースシーンの状態更新処理
	void SceneStatusUpdate();

	// フェードイン初期化
	virtual void SceneStateFadeInInitialize();
	// フェードイン更新
	virtual void SceneStateFadeInUpdate();

	// プレイフェーズ初期化
	virtual void SceneStatePlayInitialize() = 0;
	// プレイフェーズ更新
	virtual void SceneStatePlayUpdate() = 0;

	// フェードアウト初期化
	virtual void SceneStateFadeOutInitialize();
	// フェードアウト更新
	virtual void SceneStateFadeOutUpdate();

private:
	// カメラの操作
	void DebugCameraOperation();
	// UIによるカメラの操作
	void DebugCameraUI(Vector3& cameraRotate, Vector3& cameraTranslate);

	// カメラ回転操作
	void HandleCameraRotation(Vector3& cameraRotate, const POINT& delta);
	// カメラ移動操作
	void HandleCameraTranslation(Vector3& cameraTranslate, Vector3& cameraRotate, const POINT& delta);
	// カメラズーム操作
	void HandleCameraZoom(Vector3& cameraTranslate, Vector3& cameraRotate, int64_t wheelDelta);

private:
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;

protected:
	// シーンのフェーズ
	SceneState sceneState_ = SceneState::kFadeIn;
	// 次のフェーズリスト
	std::optional<SceneState> sceneStateRequest_ = std::nullopt;
	// シーンのカメラ
	std::unique_ptr<Camera> sceneCamera_ = nullptr;
	// デバッグカメラ
	std::unique_ptr<Camera> debugCamera_ = nullptr;
	// ライト
	std::unique_ptr<PunctualLight> light_ = nullptr;
	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;

	// レベルデータインポータ
	JsonLevelDataImporter levelDataImporter_;

	// デバッグカメラがアクティブかどうか
	bool isActiveDebugCamera_ = false;
};