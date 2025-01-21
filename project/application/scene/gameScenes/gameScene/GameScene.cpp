#include "GameScene.h"

#include "framework/SUGER.h"

void GameScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// レベルデータをシーンにインポート
	levelDataImporter_.Import("GameScene");

	//
	// 天球の初期化処理
	//

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(SUGER::CreateEntity("Skydome", "Skydome"));
	skydome_->SetEnableLight(false);

	// 
	// 地球の初期化処理
	// 

	earth_ = std::make_unique<Earth>();
	earth_->Initialize(SUGER::CreateEntity("Earth", "Earth"));
	earth_->CreateCollider(ColliderCategory::Earth, kSphere, 2.0f);
	earth_->SetScale(2.0f);
	earth_->GetCollider()->SetMass(200.0f);
	//
	// Playerの初期化処理
	//

	player_ = std::make_unique<Player>();
	player_->Initialize(SUGER::CreateEntity("Player", "Moon"));
	// プレイヤーのコライダーを作成
	player_->CreateCollider(ColliderCategory::Player, kSphere, 1.0f);
	player_->GetCollider()->SetMass(20.0f);

	// 
	// かけらマネージャの初期化処理
	// 

	fragmentManager_ = std::make_unique<FragmentManager>();
	fragmentManager_->Initialize();
	fragmentManager_->SetEarth(earth_.get());

	// 
	// 隕石マネージャの初期化処理
	// 

	meteoriteManager_ = std::make_unique<MeteoriteManager>();
	meteoriteManager_->Initialize(earth_.get(), fragmentManager_.get());


	//
	// たんこぶマネージャーの初期化処理
	//

	bumpManager_ = std::make_unique<BumpManager>();
	bumpManager_->Initialize();
	bumpManager_->SetPlayer(player_.get());
	// 
	player_->SetBumpManager(bumpManager_.get());


}

void GameScene::Finalize() {

}

void GameScene::SceneStatePlayInitialize() {

}

void GameScene::SceneStatePlayUpdate() {

#ifdef _DEBUG
	ImGui::Begin("GameScene");
	ImGui::Text("Restart :R");
	ImGui::End();

#endif // DEBUG

	if (SUGER::TriggerKey(DIK_R)) {
		ChangeScene("GAME");
	}

	// シェイクテスト用
	if (SUGER::TriggerKey(DIK_SPACE)) {
		sceneCamera_->Shake(15, 0.1f);
	}

	// たんこぶマネージャーの更新
	bumpManager_->Update();

	// 地球の更新
	earth_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// 隕石マネージャの更新
	meteoriteManager_->Update();

	// かけらマネージャの更新
	fragmentManager_->Update();


	// 天球の更新
	skydome_->Update();


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
	fragmentManager_->AddColliderList();

	bumpManager_->AddColliderList();
}

