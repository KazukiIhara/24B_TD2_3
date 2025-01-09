// This
#include "ModelManager.h"

void ModelManager::Initialize() {
	// 配列をクリア
	models_.clear();
	// 板ポリを作成
	CreatePlane("uvChecker.png");
}

void ModelManager::Load(const std::string& filePath) {
	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		// 読み込み済みなら早期リターン
		return;
	}
	// モデルの生成とファイルの読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(filePath);

	// モデルをmapコンテナに格納する
	models_.insert(std::make_pair(filePath, std::move(model)));
}

void ModelManager::CreateSphere(const std::string& textureFile, const std::string& directoryPath) {
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->CreateSphere(directoryPath + "/" + textureFile);
	// モデルをmapコンテナに格納する
	models_.insert(std::make_pair("Sphere", std::move(model)));
}

void ModelManager::CreatePlane(const std::string& textureFile, const std::string& directoryPath) {
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->CreatePlane(directoryPath + "/" + textureFile);
	// モデルをmapコンテナに格納する
	models_.insert(std::make_pair("Plane", std::move(model)));
}

Model* ModelManager::Find(const std::string& fileName) {
	// 読み込み済みモデルを検索
	if (models_.contains(fileName)) {
		// 読み込みモデルを戻り値としてreturn
		return models_.at(fileName).get();
	}
	// ファイル名一致なし
	return nullptr;
}

void ModelManager::Finalize() {
	// 配列をクリア
	models_.clear();
}
