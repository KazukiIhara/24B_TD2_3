#include "EntityManager.h"

#include "manager/model/ModelManager.h"
#include "3d/cameras/camera/Camera.h"
#include "3d/lights/punctualLight/PunctualLight.h"

void EntityManager::Initialize(ModelManager* modelManager) {
	// モデルマネージャのインスタンスをセット
	SetModelManager(modelManager);
	// コンテナのクリア
	ClearContainer();
}

void EntityManager::Update() {
	// 削除フラグが立っているオブジェクトを削除
	std::erase_if(entities_, [](auto& pair) {
		return pair.second && pair.second->GetIsDelete();
		});

	// 削除フラグが立っているオブジェクトを削除
	std::erase_if(skiningEntities_, [](auto& pair) {
		return pair.second && pair.second->GetIsDelete();
		});

	for (auto& pair : entities_) {
		// unique_ptrが有効かつオブジェクトが有効状態
		if (pair.second && pair.second->GetIsActive()) {
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}

	for (auto& pair : skiningEntities_) {
		// unique_ptrが有効かつオブジェクトが有効状態
		if (pair.second && pair.second->GetIsActive()) {
			// 全オブジェクトを更新
			pair.second->Update();
		}
	}
}

void EntityManager::Draw() {
	for (auto& pair : entities_) {
		// unique_ptrが有効かつオブジェクトが有効状態
		if (pair.second && pair.second->GetIsActive()) {
			// 全オブジェクトを描画
			pair.second->Draw();
		}
	}
}

void EntityManager::DrawSkining() {
	for (auto& pair : skiningEntities_) {
		// unique_ptrが有効かつオブジェクトが有効状態
		if (pair.second && pair.second->GetIsActive()) {
			// 全オブジェクトを描画
			pair.second->Draw();
		}
	}
}

void EntityManager::Finalize() {
	// コンテナのクリア
	ClearContainer();
}

void EntityManager::ClearContainer() {
	// コンテナをクリア
	entities_.clear();
}

std::string EntityManager::Create(const std::string& name, const std::string& fileName, const EulerTransform3D& transform) {
	// 重複した名前がある場合、番号を付加してユニークな名前を作成
	std::string uniqueName = name;
	int counter = 0;
	while (entities_.find(uniqueName) != entities_.end()) {
		counter++;
		uniqueName = name + "_" + std::to_string(counter);
	}

	// モデル読み込み
	modelManager_->Load(fileName);

	// スキニングアニメーションを持っているかどうか
	bool haveSkiningAnimation = modelManager_->Find(fileName)->GetHaveAnimation();

	// エンプティの生成と初期化
	std::unique_ptr<Entity> newEntity;

	if (haveSkiningAnimation) {
		// スキニングアニメーションあり
		newEntity = std::make_unique<SkiningEntity>();
	} else {
		// スキニングアニメーションなし
		newEntity = std::make_unique<Entity>();
	}

	newEntity->Initialize();

	// 名前のセット
	newEntity->SetName(uniqueName);

	// トランスフォームのセット
	newEntity->SetScale(transform.scale);
	newEntity->SetRotate(transform.rotate);
	newEntity->SetTranslate(transform.translate);

	// カメラをセット
	newEntity->SetCamera(camera_);
	// ライトをセット
	newEntity->SetLight(light_);
	// モデルをセット
	newEntity->SetModel(fileName);

	// 行列を更新
	newEntity->Update();

	if (haveSkiningAnimation) {
		// コンテナに格納
		skiningEntities_.insert(std::make_pair(uniqueName, std::move(newEntity)));
	} else {
		// コンテナに格納
		entities_.insert(std::make_pair(uniqueName, std::move(newEntity)));
	}

	// オブジェクトの名前を返す
	return uniqueName;
}

Entity* EntityManager::Find(const std::string& name) {
	// 作成済みエンティティを検索
	if (entities_.contains(name)) {
		// エンティティを返す
		return entities_.at(name).get();
		// スキニング付きのエンティティを検索
	} else if (skiningEntities_.contains(name)) {
		// スキニング付きのエンティティを返す
		return skiningEntities_.at(name).get();
	}
	// ファイル名一致なし
	return nullptr;
}

void EntityManager::SetRequiredObjects(Camera* camera, PunctualLight* punctualLight) {
	SetSceneCamera(camera);
	SetScenePunctualLight(punctualLight);
}

void EntityManager::SetSceneCamera(Camera* camera) {
	// マネージャにカメラをセット
	camera_ = camera;
	// 既に存在するオブジェクトにもセット
	for (auto& pair : entities_) {
		// unique_ptrが有効か確認
		if (pair.second) {
			// カメラをセット
			pair.second->SetCamera(camera_);
		}
	}
	for (auto& pair : skiningEntities_) {
		// unique_ptrが有効か確認
		if (pair.second) {
			// カメラをセット
			pair.second->SetCamera(camera_);
		}
	}
}

void EntityManager::SetScenePunctualLight(PunctualLight* punctualLight) {
	light_ = punctualLight;
}

void EntityManager::SetModelManager(ModelManager* modelManager) {
	modelManager_ = modelManager;
}
