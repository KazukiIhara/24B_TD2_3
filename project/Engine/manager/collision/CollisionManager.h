#pragma once

// C++
#include <list>
#include <string>

#include "3d/entityController/EntityController.h"
#include "3d/lineController/LineController.h"

// 前方宣言
class LineGroupManager;

class CollisionManager {
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	// 初期化
	void Initialize(LineGroupManager* lineManager);
	// デバッグ描画用のライングループを作成
	void InitializeColliderLineGroup();
	// 更新
	void Update();
	// 終了
	void Finalize();
	// コライダー追加
	void AddCollider(EntityController* controller);
	// コンテナをクリア
	void ClearContainer();
	// ペア同士の当たり判定をチェック
	void CheckCollisionPair(EntityController* controllerA, EntityController* controllerB);
	// すべてのコライダー同士の当たり判定チェック
	void CheckAllCollisions();
	
	// デバッグ描画用のラインコントローラを取得
	LineController* GetLineController();

private:
	// ラインマネージャをセット
	void SetLineManager(LineGroupManager* lineManager);

private:
	// ラインマネージャを受ける箱
	LineGroupManager* lineManager_ = nullptr;

private:
	// コントローラコンテナ
	std::list<EntityController*> controllers_;
	// コライダーデバッグ描画用のラインコントローラ
	std::unique_ptr<LineController> lineController_ = nullptr;
};