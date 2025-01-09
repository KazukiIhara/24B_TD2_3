#pragma once

// C++
#include <map>
#include <string>
#include <memory>

#include "VFX/particle/particle/Particle.h"

class ModelManager;
class TextureManager;
class Camera;

class ParticleManager {
public:
	ParticleManager() = default;
	~ParticleManager() = default;

	// 初期化処理
	void Initialize(ModelManager* modelManager, TextureManager* textureManager);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// 終了処理
	void Finalize();

	// コンテナをクリア
	void ClearContainer();

	// 新規板ポリパーティクル作成
	std::string CreatePlaneParticle(const std::string& name, const std::string& filePath);

	// 新規モデルパーティクル作成
	std::string CreateModelParticle(const std::string& name, const std::string& filePath);

	// 作成済みのパーティクルを検索
	Particle* Find(const std::string& name);

	// ModelManagerのセット
	void SetModelManager(ModelManager* modelManager);
	// TextureManagerのセット
	void SetTextureManager(TextureManager* textureManager);
	// Cameraのセット
	void SetSceneCamera(Camera* camera);

private:
	// パーティクルコンテナ
	std::map<std::string, std::unique_ptr<Particle>> particles_;

private:
	// モデルマネージャのインスタンス
	ModelManager* modelManager_ = nullptr;
	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = nullptr;
	// カメラのインスタンス
	Camera* camera_ = nullptr;

};