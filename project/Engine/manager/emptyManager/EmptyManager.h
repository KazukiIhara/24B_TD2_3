#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// エンプティ
#include "3d/empty/empty.h"

class EmptyManager {
public:
	EmptyManager() = default;
	~EmptyManager() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 終了
	void Finalize();

	// コンテナのクリア
	void ClearContainer();

	// エンプティ生成
	std::string Create(const std::string& name, const EulerTransform3D& transform);
	// エンプティ検索
	Empty* Find(const std::string& name);

private:
	// エンプティのコンテナ
	std::map<std::string, std::unique_ptr<Empty>> empties_;

};