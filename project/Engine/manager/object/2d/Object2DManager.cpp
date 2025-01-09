#include "Object2DManager.h"

void Object2DManager::Initialize() {
	// コンテナをクリア
	ClearContainer();
}

void Object2DManager::Update() {
	for (auto& pair : objects_) {
		if (pair.second && pair.second->GetIsActive()) {  // unique_ptrが有効かつオブジェクトが有効状態
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}
}

void Object2DManager::Draw() {
	for (auto& pair : objects_) {
		if (pair.second && pair.second->GetIsActive()) {  // unique_ptrが有効かつオブジェクトが有効状態
			// 全オブジェクトを描画
			pair.second->Draw();
		}
	}
}

void Object2DManager::Finalize() {
	// コンテナをクリア
	ClearContainer();
}

void Object2DManager::ClearContainer() {
	// コンテナをクリア
	objects_.clear();
}

std::string Object2DManager::Create(const std::string& name, const std::string& filePath) {
	// 重複した名前がある場合、番号を付加してユニークな名前を作成
	std::string uniqueName = name;
	int counter = 0;
	while (objects_.find(uniqueName) != objects_.end()) {
		counter++;
		uniqueName = name + "_" + std::to_string(counter);
	}
	// オブジェクトの生成と初期化
	std::unique_ptr<Sprite> newObject = std::make_unique<Sprite>();
	newObject->Initialize(filePath);

	// 2Dオブジェクトをコンテナに格納する
	objects_.insert(std::make_pair(uniqueName, std::move(newObject)));

	// オブジェクトの名前を返す
	return uniqueName;
}

Sprite* Object2DManager::Find(const std::string& name) {
	// 作成済みオブジェクトを検索
	if (objects_.contains(name)) {
		// オブジェクトを戻り値としてreturn
		return objects_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}
