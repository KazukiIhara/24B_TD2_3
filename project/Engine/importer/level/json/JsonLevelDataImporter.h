#pragma once

#include <string>

class JsonLevelData;

class JsonLevelDataImporter {
public:
	JsonLevelDataImporter() = default;
	~JsonLevelDataImporter() = default;

	// LevelDataをシーンにインポート
	void Import(const std::string& levelDataName);

private:

};