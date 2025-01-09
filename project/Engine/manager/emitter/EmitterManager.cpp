#include "EmitterManager.h"

void EmitterManager::Initialize() {
	ClearContainer();
}

void EmitterManager::Update() {
	for (auto& pair : emitters_) {
		if (pair.second && pair.second->GetIsActive()) {  // unique_ptr、有効フラグが有効か確認
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}
}

void EmitterManager::Finalize() {
	ClearContainer();
}

void EmitterManager::ClearContainer() {
	// コンテナをクリア
	emitters_.clear();
}

std::string EmitterManager::CreateEmitter(const std::string& name, const EulerTransform3D& transform) {// 重複した名前がある場合、番号を付加してユニークな名前を作成
	std::string uniqueName = name;
	int counter = 0;
	while (emitters_.find(uniqueName) != emitters_.end()) {
		counter++;
		uniqueName = name + "_" + std::to_string(counter);
	}
	// エミッターを新規作成
	std::unique_ptr<Emitter> newEmitter = std::make_unique<Emitter>();
	newEmitter->Initialize();
	newEmitter->SetTransform(transform);

	// エミッターをコンテナに追加
	emitters_.insert(std::make_pair(uniqueName, std::move(newEmitter)));

	return uniqueName;
}

Emitter* EmitterManager::Find(const std::string& name) {
	// 作成済みオブジェクトを検索
	if (emitters_.contains(name)) {
		// オブジェクトを戻り値としてreturn
		return emitters_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}
