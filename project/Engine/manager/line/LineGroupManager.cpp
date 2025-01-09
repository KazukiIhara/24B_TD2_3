#include "LineGroupManager.h"

void LineGroupManager::Initialize() {
	// コンテナをクリア
	ClearContainer();
}

void LineGroupManager::Update() {
	for (auto& pair : lines_) {
		if (pair.second) {  // unique_ptr、有効フラグが有効か確認
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}
}

void LineGroupManager::Draw() {
	for (auto& pair : lines_) {
		if (pair.second) {  // unique_ptr、有効フラグが有効か確認
			// 全オブジェクトを更新
			pair.second->Draw();
		}
	}
}

void LineGroupManager::Finalize() {
	// コンテナをクリア
	ClearContainer();
}

void LineGroupManager::ClearContainer() {
	lines_.clear();
}

void LineGroupManager::CreateLine(const std::string& name) {
	// 新規ライン作成
	std::unique_ptr<LineGroup> newLine = std::make_unique<LineGroup>();
	newLine->Initialize(camera_);
	// 生成したラインをコンテナに追加
	lines_.insert(std::make_pair(name, std::move(newLine)));
}

LineGroup* LineGroupManager::Find(const std::string& name) {
	// 作成済みラインを検索
	if (lines_.contains(name)) {
		// ラインを戻り値としてreturn
		return lines_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}

void LineGroupManager::SetSceneCamera(Camera* camera) {
	// マネージャにカメラをセット
	camera_ = camera;
	// 既に存在するオブジェクトにもセット
	for (auto& pair : lines_) {
		// unique_ptrが有効か確認
		if (pair.second) {
			// カメラをセット
			pair.second->SetCamera(camera_);
		}
	}
}
