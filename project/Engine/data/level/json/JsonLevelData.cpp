#include "JsonLevelData.h"
#include "framework/SUGER.h"

// C++
#include <fstream>
#include <cassert>

// Json
#include <externals/nlohmann/json.hpp>

void JsonLevelData::Load(const std::string& fullpath) {
	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として登録
	std::string name =
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHの読み込み
		if (type.compare("MESH") == 0) {

			// object3Dの場合
			if (object["object_type"] == "entity") {
				// トランスフォームのパラメータ読み込み
				nlohmann::json& transform = object["transform"];

				// 平行移動
				EulerTransform3D objectData{};
				objectData.translate.x = static_cast<float>(transform["translation"][0]);
				objectData.translate.y = static_cast<float>(transform["translation"][2]);
				objectData.translate.z = static_cast<float>(transform["translation"][1]);

				// 回転角
				objectData.rotate.x = -static_cast<float>(transform["rotation"][0]);
				objectData.rotate.y = -static_cast<float>(transform["rotation"][2]);
				objectData.rotate.z = -static_cast<float>(transform["rotation"][1]);

				// スケーリング
				objectData.scale.x = static_cast<float>(transform["scaling"][0]);
				objectData.scale.y = static_cast<float>(transform["scaling"][2]);
				objectData.scale.z = static_cast<float>(transform["scaling"][1]);

				EntityData newObject;

				// トランスフォームのセット
				newObject.transform.scale = objectData.scale;
				newObject.transform.rotate = objectData.rotate;
				newObject.transform.translate = objectData.translate;

				// 名前のセット
				newObject.objectName = object["name"];

				// モデル名がある場合
				if (object.contains("model_name")) {
					// モデル読み込み,セット
					SUGER::LoadModel(object["model_name"]);
					newObject.modelName = object["model_name"];
				}

				// 3Dオブジェクトをコンテナに格納する
				objects_.push_back(newObject);

			}
		}

	}

}

std::vector<EntityData> JsonLevelData::GetEntities()const {
	return objects_;
}
