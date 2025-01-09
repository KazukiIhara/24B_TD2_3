#include "ParticleManager.h"

#include "manager/model/ModelManager.h"
#include "manager/texture/TextureManager.h"
#include "3d/cameras/camera/Camera.h"

void ParticleManager::Initialize(ModelManager* modelManager, TextureManager* textureManager) {
	// モデルマネージャのインスタンスをセット
	SetModelManager(modelManager);
	// テクスチャマネージャのセット
	SetTextureManager(textureManager);
	// コンテナをクリア
	ClearContainer();
}

void ParticleManager::Update() {
	for (auto& pair : particles_) {
		if (pair.second && pair.second->GetIsActive()) {  // unique_ptr、有効フラグが有効か確認
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}
}

void ParticleManager::Draw() {
	for (auto& pair : particles_) {
		if (pair.second && pair.second->GetIsActive()) {  // unique_ptr、有効フラグが有効か確認
			// 全オブジェクトを更新
			pair.second->Draw();
		}
	}
}

void ParticleManager::Finalize() {
	// コンテナをクリア
	ClearContainer();
}

void ParticleManager::ClearContainer() {
	particles_.clear();
}

std::string ParticleManager::CreatePlaneParticle(const std::string& name, const std::string& filePath) {
	// 重複した名前がある場合、番号を付加してユニークな名前を作成
	std::string uniqueName = name;
	int counter = 0;
	while (particles_.find(uniqueName) != particles_.end()) {
		counter++;
		uniqueName = name + "_" + std::to_string(counter);
	}
	// テクスチャを読み込み
	textureManager_->Load(filePath);
	// 新規パーティクル作成、初期化
	std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
	newParticle->Initialize(modelManager_->Find("Plane"), camera_, filePath);
	// 板ポリタイプ
	newParticle->SetType(kPlane);
	// 生成したパーティクルをmapコンテナに格納
	particles_.insert(std::make_pair(uniqueName, std::move(newParticle)));

	return uniqueName;
}

std::string ParticleManager::CreateModelParticle(const std::string& name, const std::string& filePath) {
	// 重複した名前がある場合、番号を付加してユニークな名前を作成
	std::string uniqueName = name;
	int counter = 0;
	while (particles_.find(uniqueName) != particles_.end()) {
		counter++;
		uniqueName = name + "_" + std::to_string(counter);
	}
	// モデルを読み込み
	modelManager_->Load(filePath);
	// 新規パーティクル作成、初期化
	std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
	newParticle->Initialize(modelManager_->Find(filePath), camera_);
	// 板ポリタイプ
	newParticle->SetType(kModel);
	// 生成したパーティクルをmapコンテナに格納
	particles_.insert(std::make_pair(uniqueName, std::move(newParticle)));

	return uniqueName;
}

Particle* ParticleManager::Find(const std::string& name) {
	// 作成済みパーティクルを検索
	if (particles_.contains(name)) {
		// オブジェクトを戻り値としてreturn
		return particles_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}

void ParticleManager::SetModelManager(ModelManager* modelManager) {
	assert(modelManager);
	modelManager_ = modelManager;
}

void ParticleManager::SetTextureManager(TextureManager* textureManager) {
	assert(textureManager);
	textureManager_ = textureManager;
}

void ParticleManager::SetSceneCamera(Camera* camera) {
	assert(camera);
	camera_ = camera;
	for (auto& pair : particles_) {
		if (pair.second) {  // unique_ptrが有効か確認
			// カメラをセット
			pair.second->SetCamera(camera_);
		}
	}
}
