#include "GameScene.h"

#include "framework/SUGER.h"

void GameScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// レベルデータをシーンにインポート
	levelDataImporter_.Import("GameScene");

	//
	// Playerの初期化処理
	//

	player_ = std::make_unique<Player>();
	player_->Initialize(SUGER::CreateEntity("Player", "Player"));
	// プレイヤーのコライダーを作成
	player_->CreateCollider(kPlayer, kSphere, 1.0f);


}

void GameScene::Finalize() {

}

void GameScene::SceneStatePlayInitialize() {

}

void GameScene::SceneStatePlayUpdate() {


	// プレイヤーの更新処理

	player_->Update();


	//
	// コライダーの処理ここから
	//

	// コライダーコンテナをリセット
	SUGER::ClearColliderContainer();

	// 
	// コライダーリスト追加処理
	// 

	SUGER::AddColliderList(player_.get());

}
