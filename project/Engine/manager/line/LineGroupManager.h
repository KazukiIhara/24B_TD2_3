#pragma once

// C++
#include <map>
#include <string>
#include <memory>

#include "3d/line/LineGroup.h"

class Camera;

class LineGroupManager {
public:
	LineGroupManager() = default;
	~LineGroupManager() = default;

	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// 終了処理
	void Finalize();

	// コンテナをクリア
	void ClearContainer();

	// 新規ラインの作成
	void CreateLine(const std::string& name);

	// 作成済みのラインを検索
	LineGroup* Find(const std::string& name);

	// カメラをセット
	void SetSceneCamera(Camera* camera);


private:
	// ラインコンテナ
	std::map<std::string, std::unique_ptr<LineGroup>> lines_;

private:
	// カメラのインスタンス
	Camera* camera_ = nullptr;
};