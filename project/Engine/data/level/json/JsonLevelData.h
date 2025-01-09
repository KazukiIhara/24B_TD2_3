#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "3d/entity/Entity.h"

// TODO:レベルデータの既定クラスを作る
class JsonLevelData {
public:
	JsonLevelData() = default;
	~JsonLevelData() = default;

	// データの読み込み
	void Load(const std::string& fullpath);

	// 3Dオブジェクトコンテナのゲッター
	std::vector<EntityData> GetEntities()const;

private:
	// このレベルに存在するobject3dのデータコンテナ
	std::vector<EntityData> objects_;
};