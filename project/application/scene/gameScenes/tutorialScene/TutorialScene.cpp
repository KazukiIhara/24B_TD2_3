#include "TutorialScene.h"

#include "framework/SUGER.h"

void TutorialScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	//
	// ムーン少佐スプライト作成
	//
	moonMajarTurorial_ = std::make_unique<Object2DController>();
	// オブジェクト作成
	moonMajarTurorial_->Initialize(SUGER::Create2DObject("1_MM_Operation", "MoonMajorText/TutorialText_x1025y192.png"));
	// 各種設定
	moonMajarTurorial_->SetCutOutSize(moonMajarTextCutSize_);
	moonMajarTurorial_->SetSize(moonMajarTextCutSize_);
	moonMajarTurorial_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	moonMajarTurorial_->SetPosition(moonMajarTextPosition_);
	moonMajarTurorial_->SetIsActive(true);


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
	player_->Initialize(SUGER::CreateEntity("Player", "Earth"));
	// プレイヤーのコライダーを作成
	player_->CreateCollider(ColliderCategory::Player, kSphere, 2.0f);
	player_->GetCollider()->SetMass(20.0f);
	player_->SetScale(2.0f);
	player_->UpdateWorldTransform();


	// 
	// 月の初期化処理
	// 

	moon_ = std::make_unique<Moon>();
	moon_->Initialize(SUGER::CreateEntity("Moon", "Moon"));
	moon_->SetPlayer(player_.get());
	moon_->CreateCollider(ColliderCategory::Moon, kSphere, 1.2f);
	moon_->GetCollider()->SetMass(200.0f);
	moon_->SetDamagePieceManager(damagePieceManager_.get());
	moon_->UpdateWorldTransform();

	moon_->SetParent(player_->GetLocalTransform());
	player_->SetMoon(moon_.get());


	// 
	// ライトの初期化処理
	// 

	light_->GetPunctualLight().pointLight.position = ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_);
	light_->GetPunctualLight().pointLight.intensity = 7.0f;
	light_->GetPunctualLight().pointLight.color = { 1.0f,1.0f,0.0f,1.0f };
	light_->GetPunctualLight().pointLight.radius = 10.0f;


	light_->GetPunctualLight().spotLight.color = { 1.0f,1.0f,0.0f,1.0f };
	light_->GetPunctualLight().spotLight.position = sceneCamera_->GetWorldPos();

	light_->GetPunctualLight().spotLight.direction = Normalize(ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_) - sceneCamera_->GetWorldPos());
	light_->GetPunctualLight().spotLight.intensity = 4.0f;
	light_->GetPunctualLight().spotLight.decay = 3.0f;
	light_->GetPunctualLight().spotLight.distance = 100.0f;
	light_->GetPunctualLight().spotLight.cosAngle = 1.0f;
	light_->GetPunctualLight().spotLight.cosFalloffStart = 0.99f;



	// 
	// かけらマネージャの初期化処理
	// 

	fragmentManager_ = std::make_unique<FragmentManager>();
	fragmentManager_->Initialize();
	fragmentManager_->SetEarth(moon_.get());
	fragmentManager_->SetPlayer(player_.get());
	// 
	// 隕石マネージャの初期化処理
	// 

	meteoriteManager_ = std::make_unique<MeteoriteManager>();
	meteoriteManager_->Initialize(moon_.get(), player_.get(), fragmentManager_.get(), damagePieceManager_.get());


	//
	// たんこぶマネージャーの初期化処理
	//

	bumpManager_ = std::make_unique<BumpManager>();
	bumpManager_->Initialize();
	bumpManager_->SetPlayer(player_.get());
	bumpManager_->SetEarth(moon_.get());

	player_->SetBumpManager(bumpManager_.get());

	// 
	// UFO弾マネージャの初期化
	// 
	ufoBulletManager_ = std::make_unique<UFOBulletManager>();
	ufoBulletManager_->Initialize(player_.get());
	ufoBulletManager_->SetDamagePieceManager(damagePieceManager_.get());

	// 
	// UFOマネージャの初期化処理
	// 
	ufoManager_ = std::make_unique<UFOManager>();
	ufoManager_->Initialize(ufoBulletManager_.get());
	ufoManager_->SetDamagePieceManager(damagePieceManager_.get());
	ufoManager_->SetPlayer(player_.get());


	// ボスの初期化処理
	boss_ = std::make_unique<Boss>();
	boss_->Initialize(SUGER::CreateEntity("Boss", "Boss"));
	boss_->CreateCollider(ColliderCategory::Boss, kSphere, 6.0f);
	boss_->GetCollider()->SetMass(20000.0f);
	boss_->SetTranslate(bossPopPosition_);
	boss_->UpdateWorldTransform();
	boss_->SetUFOBulletManager(ufoBulletManager_.get());
	boss_->SetPlayer(player_.get());

	// 板ポリパーティクルの作成
	SUGER::CreateParticle("dustParticle", ParticleType::kPlane, "circle.png");
	//SUGER::CreateParticle("fragmentParticle", ParticleType::kModel,"Fragment");
	SUGER::CreateParticle("fragmentParticle", ParticleType::kPlane, "dust.png");
	SUGER::CreateParticle("earthDustParticle", ParticleType::kPlane, "dust.png");
	SUGER::CreateParticle("explosionDustParticle", ParticleType::kPlane, "dust.png");
	SUGER::CreateParticle("bumpParticle", ParticleType::kPlane, "circle.png");
	SUGER::CreateParticle("ShotDustParticle", ParticleType::kPlane, "dust.png");
	SUGER::CreateParticle("MoonParticle", ParticleType::kModel, "Moon");


	moon_->SetPraticle();
	moon_->UpdateWorldTransform();


	//for (auto& tex : tutorialTexs_) {
	//	tex = std::make_unique<Object2DController>();
	//}
	//tutorialTexs_[0]->Initialize(SUGER::Create2DObject("0_Tutorialtex", "TutorialImages/TutorialImage_00.png"));
	//tutorialTexs_[1]->Initialize(SUGER::Create2DObject("0_Tutorialtex", "TutorialImages/TutorialImage_01.png"));
	//tutorialTexs_[2]->Initialize(SUGER::Create2DObject("0_Tutorialtex", "TutorialImages/TutorialImage_02.png"));
	//tutorialTexs_[3]->Initialize(SUGER::Create2DObject("0_Tutorialtex", "TutorialImages/TutorialImage_03.png"));
	//tutorialTexs_[4]->Initialize(SUGER::Create2DObject("0_Tutorialtex", "TutorialImages/TutorialImage_04.png"));
	//for (auto& tex : tutorialTexs_) {
	//	tex->SetIsActive(false);
	//}
	//tutorialTexs_[0]->SetIsActive(true);


}

void TutorialScene::Finalize() {

}

void TutorialScene::SceneStatePlayInitialize() {

}

void TutorialScene::SceneStatePlayUpdate() {

	// ライトの座標
	light_->GetPunctualLight().pointLight.position = ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_);
	light_->GetPunctualLight().spotLight.direction = Normalize(ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_) - sceneCamera_->GetWorldPos());

	player_->Update();
	moon_->Update();
	// 天球の更新
	skydome_->Update();
	// ダメージ破片の更新
	damagePieceManager_->Update();

	moonMajarTurorial_->SetLeftTop(Vector2(0.0f, static_cast<float>(currentPage_) * moonMajarTextCutSize_.y));
	// ボタンでページ送り
	if (SUGER::TriggerButton(0, ButtonB) || SUGER::TriggerKey(DIK_Z)) {
		currentPage_++;
	}

	if (currentPage_ == 15) {
		sceneStateRequest_ = SceneState::kFadeOut;
	}

}

void TutorialScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		ChangeScene("GAME");
		fade_->Stop();
	}
}
