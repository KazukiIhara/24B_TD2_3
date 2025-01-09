#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyHedder
#include "2d/sprite/Sprite.h"

class Object2DManager {
public:
	Object2DManager() = default;
	~Object2DManager() = default;

	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// 終了処理
	void Finalize();
	// コンテナのクリア
	void ClearContainer();

	// オブジェクト作成
	std::string Create(const std::string& name, const std::string& filePath);

	// オブジェクト検索
	Sprite* Find(const std::string& name);

private:
	// 2Dオブジェクトデータコンテナ
	std::map<std::string, std::unique_ptr<Sprite>> objects_;

};