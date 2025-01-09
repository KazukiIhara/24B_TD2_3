#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// エンティティ
#include "3d/skiningEntity/SkiningEntity.h"

class ModelManager;
class PunctualLight;
class Camera;

class EntityManager {
public:
	EntityManager() = default;
	~EntityManager() = default;

	// 初期化
	void Initialize(ModelManager* modelManager);
	// 更新
	void Update();
	// 描画処理
	void Draw();
	// スキニング付きエンティティの描画
	void DrawSkining();
	// 終了
	void Finalize();

	// コンテナのクリア
	void ClearContainer();

	// エンティティ生成
	std::string Create(const std::string& name, const std::string& fileName, const EulerTransform3D& transform);
	// エンティティ検索
	Entity* Find(const std::string& name);

	// ライトとカメラをまとめてセット
	void SetRequiredObjects(Camera* camera, PunctualLight* punctualLight);
	// カメラをセット
	void SetSceneCamera(Camera* camera);
	// ライトをセット
	void SetScenePunctualLight(PunctualLight* punctualLight);

private:
	// モデルマネージャのセット
	void SetModelManager(ModelManager* modelManager);

private:
	// エンティティのコンテナ
	std::map<std::string, std::unique_ptr<Entity>> entities_;
	std::map<std::string, std::unique_ptr<Entity>> skiningEntities_;
private:
	// モデルマネージャのインスタンス
	ModelManager* modelManager_ = nullptr;
	// カメラのインスタンス
	Camera* camera_ = nullptr;
	// ライトのインスタンス
	PunctualLight* light_ = nullptr;

};