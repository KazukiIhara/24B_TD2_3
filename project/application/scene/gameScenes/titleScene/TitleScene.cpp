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
	player_->SetTranslate({ 6.0f,-1.0f,7.0f });

	// 
	// 月の初期化処理
	// 

	float dir = DegreesToRadians(inclination_);

	moon_ = std::make_unique<Moon>();
	moon_->Initialize(SUGER::CreateEntity("Moon", "Earth"));
	moon_->SetParent(player_->GetLocalTransform());
	moon_->CreateCollider(ColliderCategory::Moon, kSphere, 2.0f);
	moon_->GetCollider()->SetMass(200.0f);
	moon_->SetDamagePieceManager(damagePieceManager_.get());
	moon_->SetTranslate(Vector3{ -10.5f, -0.5f, 0 });
	moon_->SetRotateZ(dir);
	moon_->SetScale(1.5f);


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


	startUI_ = std::make_unique<Object2DController>();
	startUI_->Initialize(SUGER::Create2DObject("0_A", "TitleText/StartUI.png"));
	startUI_->SetAnchorPoint({ 0.5f,0.5f });
	startUI_->SetPosition({ 1920 / 2,870 });

	titleTextAni_ = std::make_unique<Object2DController>();
	titleTextAni_->Initialize(SUGER::Create2DObject("0_TitleTexAni", "TitleText/StartUI.png"));
	titleTextAni_->SetAnchorPoint({ 0.5f,0.5f });
	titleTextAni_->SetPosition({ 1920 / 2,870 });

	constAniSize_ = titleTextAni_->GetSize();
	anisize_ = constAniSize_;

	titleText_ = std::make_unique<Object2DController>();
	titleText_->Initialize(SUGER::Create2DObject("0_TitleTex", "TitleText/TitleText.png"));
	titleText_->SetAnchorPoint({ 0.5f,0.5f });
	titleText_->SetPosition(Vector2(960.0f, 300.0f));
	titleText_->SetSize(titleText_->GetSize() * 1.5f);

	F11UI_ = std::make_unique<Object2DController>();
	F11UI_->Initialize(SUGER::Create2DObject("0_TitleTex", "ButtonUI/F11.png"));
	//F11UI_->SetAnchorPoint({ 0.5f,0.5f });
	//F11UI_->SetPosition(Vector2(960.0f, 300.0f));
	//titleText_->SetSize(titleText_->GetSize() * 1.5f);

	aniTimer_ = aniTime_;
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

	if (aniTimer_ > 0) {
		aniTimer_--;
		anisize_.x += 4.0f;
		anisize_.y += 4.0f;
		aniAlpha_ -= 0.05f;
	} else {
		anisize_ = constAniSize_;
		aniAlpha_ = 1.0f;
		aniTimer_ = aniTime_;
	}

	titleTextAni_->SetColor(Vector4(1.0f, 1.0f, 1.0f, aniAlpha_));
	titleTextAni_->SetSize(anisize_);

	// ダメージ破片の更新
	damagePieceManager_->Update();

	// 天球の更新
	skydome_->Update();


	if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
		sceneStateRequest_ = SceneState::kFadeOut;
		startUI_->SetIsActive(false);
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
