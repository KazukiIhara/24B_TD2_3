#include "GameScene.h"

#include "framework/SUGER.h"

void GameScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// レベルデータをシーンにインポート
	levelDataImporter_.Import("GameScene");


	// 
	// 地球の初期化処理
	// 

	earth_ = std::make_unique<Earth>();
	earth_->Initialize(SUGER::CreateEntity("Earth", "Earth"));
	earth_->CreateCollider(ColliderCategory::Earth, kSphere, 2.0f);
	earth_->GetCollider()->SetMass(200.0f);
	//
	// Playerの初期化処理
	//

	player_ = std::make_unique<Player>();
	player_->Initialize(SUGER::CreateEntity("Player", "Player"));
	// プレイヤーのコライダーを作成
	player_->CreateCollider(ColliderCategory::Player, kSphere, 1.0f);


	// 
	// かけらマネージャの初期化処理
	// 

	fragmentManager_ = std::make_unique<FragmentManager>();
	fragmentManager_->Initialize();

	// 
	// 隕石マネージャの初期化処理
	// 

	meteoriteManager_ = std::make_unique<MeteoriteManager>();
	meteoriteManager_->Initialize(earth_.get(), fragmentManager_.get());

	// とりあえず一つ追加
	meteoriteManager_->AddMeteorite();


}

void GameScene::Finalize() {

}

void GameScene::SceneStatePlayInitialize() {

}

void GameScene::SceneStatePlayUpdate() {

#ifdef _DEBUG
	ImGui::Begin("GameScene");
	ImGui::Text("Restart :R");
	ImGui::Text("Move :WASD");

	ImGui::End();

	if (SUGER::TriggerKey(DIK_R)) {
		ChangeScene("GAME");
	}

#endif // DEBUG

	// 地球の更新
	earth_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// 隕石マネージャの更新
	meteoriteManager_->Update();

	// かけらマネージャの更新
	fragmentManager_->Update();

	//
	// コライダーの処理ここから
	//

	// コライダーコンテナをリセット
	SUGER::ClearColliderContainer();

	// 
	// コライダーリスト追加処理
	// 

	SUGER::AddColliderList(player_.get());
	SUGER::AddColliderList(earth_.get());
	meteoriteManager_->AddColliderList();

}
