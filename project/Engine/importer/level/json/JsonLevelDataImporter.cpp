#include "JsonLevelDataImporter.h"

#include "data/level/json/JsonLevelData.h"

#include "framework/SUGER.h"

void JsonLevelDataImporter::Import(const std::string& levelDataName) {
	// レベルデータマネージャからロード済みのレベルデータを取得
	JsonLevelData* jsonLevelData = SUGER::FindJsonLevelData(levelDataName);
	// 配置されているすべてのオブジェクトを生成
	for (EntityData object : jsonLevelData->GetEntities()) {
		SUGER::CreateEntity(object.objectName, object.modelName, object.transform);
	}
}