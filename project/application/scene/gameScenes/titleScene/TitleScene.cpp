#include "TitleScene.h"

#include "framework/SUGER.h"

void TitleScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();


	//
	// 天球の初期化処理
	//

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(SUGER::CreateEntity("Skydome", "Skydome"));
	skydome_->SetEnableLight(false);
	skydome_->SetScale(100.0f);
	skydome_->GetUVTransform().scale = { 10.0f,10.0f };

	//
	//
	//
	damagePieceManager_ = std::make_unique<DamagePieceManager>();
	damagePieceManager_->Initialize();

	// 
	// 地球の初期化処理
	// 

	earth_ = std::make_unique<Earth>();
	earth_->Initialize(SUGER::CreateEntity("Earth", "Earth"));
	earth_->CreateCollider(ColliderCategory::Earth, kSphere, 2.0f);
	earth_->SetScale(2.0f);
	earth_->GetCollider()->SetMass(200.0f);
	earth_->SetDamagePieceManager(damagePieceManager_.get());
	earth_->SetTranslate({ 0,0,3 });
	//
	// Playerの初期化処理
	//

	player_ = std::make_unique<Player>();
	player_->Initialize(SUGER::CreateEntity("Player", "Moon"));
	// プレイヤーのコライダーを作成
	player_->CreateCollider(ColliderCategory::Player, kSphere, 1.0f);
	player_->GetCollider()->SetMass(20.0f);
	player_->SetTranslate(Vector3{ -10.5f, -0.5f, 0 });

	light_->GetPunctualLight().pointLight.position = player_->GetTranslate();
	light_->GetPunctualLight().pointLight.intensity = 7.0f;
	light_->GetPunctualLight().pointLight.color = { 1.0f,1.0f,0.0f,1.0f };
	light_->GetPunctualLight().pointLight.radius = 10.0f;


	light_->GetPunctualLight().spotLight.color = { 1.0f,1.0f,0.0f,1.0f };
	light_->GetPunctualLight().spotLight.position = sceneCamera_->GetWorldPos();

	light_->GetPunctualLight().spotLight.direction = Normalize(player_->GetTranslate() - sceneCamera_->GetWorldPos());
	light_->GetPunctualLight().spotLight.intensity = 4.0f;
	light_->GetPunctualLight().spotLight.decay = 3.0f;
	light_->GetPunctualLight().spotLight.distance = 100.0f;
	light_->GetPunctualLight().spotLight.cosAngle = 1.0f;
	light_->GetPunctualLight().spotLight.cosFalloffStart = 0.99f;

	sceneCamera_->SetTranslate({-10.5f,0.25f,-3.5});
	sceneCamera_->SetRotate({0.0f,0.6f,0}); 
	
}

void TitleScene::Finalize() {

}

void TitleScene::SceneStatePlayInitialize() {

}

void TitleScene::SceneStatePlayUpdate() {
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	if (ImGui::Button("ChangeScene")) {
		sceneStateRequest_ = SceneState::kFadeOut;
	}
	ImGui::End();
#endif // _DEBUG

	// 地球の更新
	earth_->UpdateTitle();

	if (earth_->GetIsHit()) {
		if (earth_->GetHitLevel() == 1) {
			sceneCamera_->Shake(15.0f, 0.5f);
		}
		else if (earth_->GetHitLevel() == 2) {
			sceneCamera_->Shake(25.0f, 1.5f);
		}
		earth_->SetIsHit(false);
	}


	// ライトの座標
	//light_->GetPunctualLight().pointLight.position = player_->GetTranslate();
	//light_->GetPunctualLight().spotLight.direction = Normalize(player_->GetTranslate() - sceneCamera_->GetWorldPos());


	// プレイヤーの更新処理
	//player_->Update();

	// ダメージ破片の更新
	damagePieceManager_->Update();

	// 天球の更新
	skydome_->Update();


	if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
		sceneStateRequest_ = SceneState::kFadeOut;
	}
}

void TitleScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		ChangeScene("TUTORIAL");
		fade_->Stop();
	}
}
