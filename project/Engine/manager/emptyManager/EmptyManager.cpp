#include "EmptyManager.h"

void EmptyManager::Initialize() {
	// コンテナをクリア
	ClearContainer();
}

void EmptyManager::Update() {
	// 削除フラグが立っているオブジェクトを削除
	std::erase_if(empties_, [](auto& pair) {
		return pair.second && pair.second->GetIsDelete();
		});

	for (auto& pair : empties_) {
		// unique_ptrが有効かつオブジェクトが有効状態
		if (pair.second && pair.second->GetIsActive()) {
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}
}

void EmptyManager::Finalize() {
	// コンテナをクリア
	ClearContainer();
}

void EmptyManager::ClearContainer() {
	// コンテナをクリア
	empties_.clear();
}

std::string EmptyManager::Create(const std::string& name, const EulerTransform3D& transform) {
	// 重複した名前がある場合、番号を付加してユニークな名前を作成
	std::string uniqueName = name;
	int counter = 0;
	while (empties_.find(uniqueName) != empties_.end()) {
		counter++;
		uniqueName = name + "_" + std::to_string(counter);
	}

	// エンプティの生成と初期化
	std::unique_ptr<Empty> newEmpty = std::make_unique<Empty>();
	newEmpty->Initialize();

	// 名前のセット
	newEmpty->SetName(uniqueName);

	// トランスフォームのセット
	newEmpty->SetScale(transform.scale);
	newEmpty->SetRotate(transform.rotate);
	newEmpty->SetTranslate(transform.translate);

	// 行列を更新
	newEmpty->Update();

	// コンテナに格納
	empties_.insert(std::make_pair(uniqueName, std::move(newEmpty)));

	// オブジェクトの名前を返す
	return uniqueName;
}

Empty* EmptyManager::Find(const std::string& name) {
	// 作成済みオブジェクトを検索
	if (empties_.contains(name)) {
		// オブジェクトを戻り値としてreturn
		return empties_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}