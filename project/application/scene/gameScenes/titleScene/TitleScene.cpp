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
	// Playerの初期化処理
	//

	player_ = std::make_unique<Player>();
	player_->Initialize(SUGER::CreateEntity("Player", "Moon"));
	// プレイヤーのコライダーを作成
	player_->CreateCollider(ColliderCategory::Player, kSphere, 1.0f);
	player_->SetScale(2.0f);
	player_->GetCollider()->SetMass(20.0f);
	player_->SetTranslate(Vector3{ -10.5f, -0.5f, 0 });


	// 
	// 月の初期化処理
	// 

	moon_ = std::make_unique<Moon>();
	moon_->Initialize(SUGER::CreateEntity("Moon", "Earth"));
	moon_->SetParent(player_->GetLocalTransform());
	moon_->CreateCollider(ColliderCategory::Moon, kSphere, 2.0f);
	moon_->GetCollider()->SetMass(200.0f);
	moon_->SetDamagePieceManager(damagePieceManager_.get());
	moon_->SetTranslate({ 0,0,3 });


	//
	// ライトの処理
	//

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

	sceneCamera_->SetTranslate({ -10.5f,0.25f,-3.5 });
	sceneCamera_->SetRotate({ 0.0f,0.6f,0 });


	A_UI_ = std::make_unique<Object2DController>();
	A_UI_->Initialize(SUGER::Create2DObject("A", "A.png"));
	A_UI_->SetAnchorPoint({ 0.5f,0.5f });
	A_UI_->SetPosition({ 1920 / 2,870 });
	A_UI_->SetSize({ 64 * 3,64 * 3 });
}


void TitleScene::Finalize() {
	//BGMの停止
	SUGER::StopWaveLoopSound("Title.wav");
}

void TitleScene::SceneStatePlayInitialize() {
	// BGMの再生
	SUGER::PlayWaveLoopSound("Title.wav");
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
	moon_->UpdateTitle();

	if (player_->GetIsHit()) {
		if (player_->GetHitLevel() == 1) {
			sceneCamera_->Shake(15.0f, 0.5f);
		} else if (player_->GetHitLevel() == 2) {
			sceneCamera_->Shake(25.0f, 1.5f);
		}
		player_->SetIsHit(false);
	}


	if (++time_ >= 40) {
		time_ = 0;
		clock_ *= -1;
	}
	if (clock_ == 1) {
		A_UI_->SetIsActive(true);
	} else {
		A_UI_->SetIsActive(false);
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
		A_UI_->SetIsActive(false);
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
