#include "JsonLevelDataManager.h"

void JsonLevelDataManager::Initialize() {
	// コンテナをクリア
	levelDatas_.clear();
}

void JsonLevelDataManager::Finalize() {
	// コンテナをクリア
	levelDatas_.clear();
}

void JsonLevelDataManager::Load(const std::string& fileName, const std::string& directoryPath) {
	// 新しいレベルデータの生成
	std::unique_ptr<JsonLevelData> newLevel = std::make_unique<JsonLevelData>();
	// フルパス
	const std::string fullPath = directoryPath + fileName + ".json";
	// レベルをjsonから読み込み
	newLevel->Load(fullPath);
	// レベルデータをコンテナに格納
	levelDatas_.insert(std::make_pair(fileName, std::move(newLevel)));
}

JsonLevelData* JsonLevelDataManager::Find(const std::string& name) {
	// 作成済みレベルデータを検索
	if (levelDatas_.contains(name)) {
		// レベルデータを戻り値としてreturn
		return levelDatas_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}
