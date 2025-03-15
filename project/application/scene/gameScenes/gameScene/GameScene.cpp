#include "GameScene.h"

#include "framework/SUGER.h"

#include "random/Random.h"

void GameScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// レベルデータをシーンにインポート
	levelDataImporter_.Import("GameScene");



	//
	// forDebug
	//

	/*plane_ = std::make_unique<EntityController>();

	plane_->Initialize(SUGER::CreateEntity("Plane", "Plane"));
	plane_->SetRotateY(std::numbers::pi_v<float>);
	plane_->SetScale(100.0f);
	plane_->SetShinness(0.0f);*/


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
	SUGER::CreateParticle("UFOBulletParticle", ParticleType::kModel, "UFO_Bullet");


	moon_->SetPraticle();
	moon_->UpdateWorldTransform();
	//
	// スプライトの初期化処理
	//

	for (uint32_t i = 0; i < 4; i++) {
		earthHPUI_[i] = std::make_unique<Object2DController>();
	}

	std::string earthHpUiString = "Earth_HP_UI/Earth_HP_UI";

	earthHPUI_[0]->Initialize(SUGER::Create2DObject("3_earthHPUI", earthHpUiString + "_100%.png"));
	earthHPUI_[1]->Initialize(SUGER::Create2DObject("2_earthHPUI", earthHpUiString + "_75%.png"));
	earthHPUI_[2]->Initialize(SUGER::Create2DObject("1_earthHPUI", earthHpUiString + "_50%.png"));
	earthHPUI_[3]->Initialize(SUGER::Create2DObject("0_earthHPUI", earthHpUiString + "_25%.png"));

	earthUISize_ = earthHPUI_[0]->GetSize();


	SUGER::AddGrobalDataGroup("UI");
	SUGER::AddGrobalDataItem("UI", "EarthUIPosX", earthUIPosition_.x);
	SUGER::AddGrobalDataItem("UI", "EarthUIPosY", earthUIPosition_.y);
	SUGER::AddGrobalDataItem("UI", "EarthUISizeX", earthUISize_.x);
	SUGER::AddGrobalDataItem("UI", "EarthUISizeY", earthUISize_.y);

	earthUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosX");
	earthUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosY");

	earthUISize_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUISizeX");
	earthUISize_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUISizeY");


	for (uint32_t i = 0; i < 4; i++) {
		earthHPUI_[i]->SetPosition(earthUIPosition_);
		earthHPUI_[i]->SetSize(earthUISize_);
		earthHPUI_[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		earthHPUI_[i]->SetIsActive(false);
	}

	earthHPUI_[0]->SetIsActive(true);

	for (uint32_t i = 0; i < 3; i++) {
		earthHpNumUI_[i] = std::make_unique<Object2DController>();
		earthHpNumUI_[i]->Initialize(SUGER::Create2DObject("0_earthHpNumUI", "Number/Number_x128y192.png"));
		earthHpNumUI_[i]->SetCutOutSize(numberTextureSize_);
		earthHpNumUI_[i]->SetSize(numberTextureSize_ / 2.0f);
	}

	earthHpNum_ = SplitDigits(static_cast<uint32_t>(player_->GetHp()));

	SUGER::AddGrobalDataItem("UI", "EarthUINumPosX", earthHpNumUIPosition_.x);
	SUGER::AddGrobalDataItem("UI", "EarthUINumPosY", earthHpNumUIPosition_.y);

	earthHpNumUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUINumPosX");
	earthHpNumUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUINumPosY");

	// UIのポジションを決定
	earthHpNumUI_[0]->SetPosition(earthHpNumUIPosition_ - Vector2(numGap_, 0.0f));
	earthHpNumUI_[1]->SetPosition(earthHpNumUIPosition_);
	earthHpNumUI_[2]->SetPosition(earthHpNumUIPosition_ + Vector2(numGap_, 0.0f));

	// 数字を基に左上をずらして描画
	for (uint32_t i = 0; i < 3; i++) {
		earthHpNumUI_[i]->SetLeftTop(Vector2(earthHpNum_[i] * numberTextureSize_.x, 0.0f));
	}

	//
	// 日数表示
	//

	for (uint32_t i = 0; i < 3; i++) {
		currentDaysNumUI_[i] = std::make_unique<Object2DController>();
		currentDaysNumUI_[i]->Initialize(SUGER::Create2DObject("0_currentDays", "Number/Number_x128y192.png"));
		currentDaysNumUI_[i]->SetCutOutSize(numberTextureSize_);
		currentDaysNumUI_[i]->SetSize(numberTextureSize_ / 2.0f);
	}

	SUGER::AddGrobalDataItem("UI", "CurrentDaysNumPosX", currentDaysPosition_.x);
	SUGER::AddGrobalDataItem("UI", "CurrentDaysNumPosY", currentDaysPosition_.y);

	currentDaysPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "CurrentDaysNumPosX");
	currentDaysPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "CurrentDaysNumPosY");

	currentDaysNumUI_[0]->SetPosition(currentDaysPosition_ - Vector2(numGap_, 0.0f));
	currentDaysNumUI_[1]->SetPosition(currentDaysPosition_);
	currentDaysNumUI_[2]->SetPosition(currentDaysPosition_ + Vector2(numGap_, 0.0f));

	// 描画フラグ初期化
	currentDaysNumUI_[0]->SetIsActive(false);
	currentDaysNumUI_[1]->SetIsActive(false);
	currentDaysNumUI_[2]->SetIsActive(true);

	//
	// 年数表示
	//

	for (uint32_t i = 0; i < 3; i++) {
		currentBossBattleNumUI_[i] = std::make_unique<Object2DController>();
		currentBossBattleNumUI_[i]->Initialize(SUGER::Create2DObject("0_currentYears", "Number/Number_x128y192.png"));
		currentBossBattleNumUI_[i]->SetCutOutSize(numberTextureSize_);
		currentBossBattleNumUI_[i]->SetSize(numberTextureSize_ / 2.0f);
	}

	SUGER::AddGrobalDataItem("UI", "CurrentYearsNumPosX", currentYearsPosition_.x);
	SUGER::AddGrobalDataItem("UI", "CurrentYearsNumPosY", currentYearsPosition_.y);

	currentYearsPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "CurrentYearsNumPosX");
	currentYearsPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "CurrentYearsNumPosY");

	currentBossBattleNumUI_[0]->SetPosition(currentYearsPosition_ - Vector2(numGap_, 0.0f));
	currentBossBattleNumUI_[1]->SetPosition(currentYearsPosition_);
	currentBossBattleNumUI_[2]->SetPosition(currentYearsPosition_ + Vector2(numGap_, 0.0f));

	// 描画フラグ初期化
	currentBossBattleNumUI_[0]->SetIsActive(false);
	currentBossBattleNumUI_[1]->SetIsActive(false);
	currentBossBattleNumUI_[2]->SetIsActive(false);

	// 
	// シンボル
	// 

	for (uint32_t i = 0; i < 2; i++) {
		symbolUI_[i] = std::make_unique<Object2DController>();
	}

	symbolUI_[0]->Initialize(SUGER::Create2DObject("0_Days", "Number/Symbol_x128y192.png"));
	symbolUI_[1]->Initialize(SUGER::Create2DObject("0_Years", "Number/Symbol_x128y192.png"));

	for (uint32_t i = 0; i < 2; i++) {
		symbolUI_[i]->SetCutOutSize(numberTextureSize_);
		symbolUI_[i]->SetSize(numberTextureSize_ / 2.0f);
	}

	symbolUI_[0]->SetLeftTop(Vector2(8 * numberTextureSize_.x, 0.0f));
	symbolUI_[0]->SetPosition(currentDaysPosition_ + Vector2(numGap_ * 2.0f, 8.0f));
	symbolUI_[1]->SetLeftTop(Vector2(9 * numberTextureSize_.x, 0.0f));
	symbolUI_[1]->SetPosition(currentYearsPosition_ + Vector2(numGap_ * 2.0f, 8.0f));
	symbolUI_[1]->SetIsActive(false);



	//
	// ムーン少佐
	// 

	moonMajarSprite_ = std::make_unique<Object2DController>();
	moonMajarSprite_->Initialize(SUGER::Create2DObject("MoonMajarUI", "MoonMajorText/MoonMajorText_Game.png"));
	moonMajarSprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	moonMajarSprite_->SetPosition(moonMajarPosition_);


	currentDays_ = 0;


	//
	// ボタンUI
	//
	buttomUI_ = std::make_unique<Object2DController>();
	buttomUI_->Initialize(SUGER::Create2DObject("0_Botton", "ButtonUI/ShotUI_x512y96.png"));
	buttomUI_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	buttomUI_->SetPosition(buttomUIPosition_);
	buttomUI_->SetSize(buttomUiSize_);
	buttomUI_->SetCutOutSize(buttomUiSize_);
	buttomUI_->SetLeftTop(Vector2(0.0f, 0.0f));
	buttomUI_->SetColor({ 1,1,1,0.8f });

	// 
	// ホワイトフェード
	// 

	whiteFade_ = std::make_unique<Object2DController>();
	whiteFade_->Initialize(SUGER::Create2DObject("200_White", "white.png"));
	whiteFade_->SetColor(Vector4(1.0f, 1.0f, 1.0f, alpha_));
	whiteFade_->SetSize(Vector2(1920, 1080));

	InitializeUI(scoreUI_, "0decimalPoint", "ResultText/ResultNumber_x48y96.png", numberTextureSize2_);


	// スコア(テキスト)
	resultScore_ = std::make_unique<Object2DController>();
	resultScore_->Initialize(SUGER::Create2DObject("0result", "ResultText/ResultScore.png"));
	resultScore_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultScore_->SetPosition(resultScorePos_);
	resultScore_->SetSize(scoreSize * 0.8f);

}

void GameScene::Finalize() {
	// BGMの停止
	SUGER::StopWaveLoopSound("GameScene.wav");
	SUGER::StopWaveLoopSound("BossBGM.wav");
}

void GameScene::SceneStatePlayInitialize() {
	// BGMの再生
	SUGER::PlayWaveLoopSound("GameScene.wav");
}

void GameScene::SceneStatePlayUpdate() {

#ifdef _DEBUG
	ImGui::Begin("GameScene");
	ImGui::Text("Restart :R");
	ImGui::Text("CurrentDays: %u", currentDays_);
	ImGui::Text("CurrentYears: %u", currentYears_);
	ImGui::End();

	earthUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosX");
	earthUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosY");

	earthUISize_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUISizeX");
	earthUISize_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUISizeY");

	earthHpNumUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUINumPosX");
	earthHpNumUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUINumPosY");

	currentDaysPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "CurrentDaysNumPosX");
	currentDaysPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "CurrentDaysNumPosY");

	currentYearsPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "CurrentYearsNumPosX");
	currentYearsPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "CurrentYearsNumPosY");

#endif // DEBUG

#ifdef _DEBUG
	if (SUGER::TriggerKey(DIK_R)) {
		ChangeScene("GAME");
	}

	ImGui::Begin("PointLightSetting");
	PointLightForGPU& pointLight = light_->GetPunctualLight().pointLight;
	ImGui::ColorEdit3("color", &pointLight.color.x);
	ImGui::DragFloat("intensity", &pointLight.intensity);
	ImGui::DragFloat("radius", &pointLight.radius);
	ImGui::DragFloat("decay", &pointLight.decay);
	ImGui::End();

	ImGui::Begin("SpotLightSetting");
	SpotLightForGPU& spotLight = light_->GetPunctualLight().spotLight;
	ImGui::ColorEdit3("color", &spotLight.color.x);
	ImGui::DragFloat("intensity", &spotLight.intensity);
	ImGui::DragFloat("decay", &spotLight.decay, 0.01f);
	ImGui::DragFloat("cosAngle", &spotLight.cosAngle, 0.01f);
	ImGui::DragFloat("cosFallossStart", &spotLight.cosFalloffStart, 0.01f);
	ImGui::End();

#endif // _DEBUG


	// ゲームオーバー確認処理
	if (gameOverAnimationTimer_ == 180.0f) {
		// スコアを保存
		GetGameData().days_ = currentDays_;
		GetGameData().years_ = currentYears_;
		GetGameData().score_ = player_->GetScoreData().score_;
		GetGameData().fragmentNum_ = player_->GetScoreData().fragmentNum_;
		GetGameData().meteoriteNum_ = player_->GetScoreData().meteoriteNum_;
		GetGameData().ufoNum_ = player_->GetScoreData().ufoNum_;
		GetGameData().bossDeath_ = false;

		// フェードリクエスト
		sceneStateRequest_ = SceneState::kFadeOut;
	}

	if (boss_->IsBossKill()) {
		// スコアを保存
		GetGameData().days_ = currentDays_;
		GetGameData().years_ = currentYears_;
		GetGameData().score_ = player_->GetScoreData().score_;
		GetGameData().fragmentNum_ = player_->GetScoreData().fragmentNum_;
		GetGameData().meteoriteNum_ = player_->GetScoreData().meteoriteNum_;
		GetGameData().ufoNum_ = player_->GetScoreData().ufoNum_;
		GetGameData().bossDeath_ = true;


		// フェードリクエスト
		sceneStateRequest_ = SceneState::kFadeOut;
	}

	// シェイクテスト用
	if (SUGER::TriggerKey(DIK_SPACE)) {
		//	sceneCamera_->Shake(15.0f, 0.5f);
	}

	if (player_->GetHp() <= 0) {
		gameOverAnimation_ = true;
		sceneCamera_->Shake(3.0f, 1.0f);
	}


	if (gameOverAnimation_) {
		gameOverAnimationTimer_++;

		if (gameOverAnimationTimer_ > 180) {
			gameOverAnimationTimer_ = 180;
		}
		alpha_ = gameOverAnimationTimer_ / gameOverAnimationTime_;
		whiteFade_->SetColor(Vector4(Vector4(1.0f, 1.0f, 1.0f, alpha_)));

	} else {

		// スコア用のタイマー更新
		scoreTimer_++;

		// 地球の更新
		moon_->Update();

		if (!isBossFightStart_ && !isBossFight_) {
			// 経過日数を加算
			if (scoreTimer_ == moon_->GetAroundFrame() / 15.0f) {
				currentDays_++;
				scoreTimer_ = 0.0f;
			}
		}

		if (player_->GetIsHit()) {
			if (player_->GetHitLevel() == 1) {
				sceneCamera_->Shake(15.0f, 0.5f);
			} else if (player_->GetHitLevel() == 2) {
				sceneCamera_->Shake(25.0f, 1.5f);
			}
			player_->SetIsHit(false);
		}

		// ボス登場時pr
		if (isBossFightStart_) {
			bossFightStartTimer_++;

			// ボス登場シーン

			if (bossFightStartTimer_ == bossFightStartTime_) {
				isBossFight_ = true;
				isBossFightStart_ = false;
				boss_->RequestRoot();
				SUGER::StopWaveLoopSound("Warning.wav");
				// ボスBGM
				SUGER::PlayWaveLoopSound("BossBGM.wav");

				bossBattleTimer_ = 120;
			}
			Vector3 bossPos = Lerp(bossPopPosition_, bossBattleBeginPosition_, bossFightStartTimer_ / bossFightStartTime_);
			boss_->SetTranslate(bossPos);
		} else {
			// たんこぶマネージャーの更新
			bumpManager_->Update();

			// 隕石マネージャの更新
			meteoriteManager_->Update();

			// かけらマネージャの更新
			fragmentManager_->Update();

			// UFOマネージャの更新
			ufoManager_->Update();
		}



		// 通常戦
		if (!isBossFight_ && !isBossFightStart_) {

			if (currentDays_ == 365) {
				currentYears_++;
				currentDays_ = 0;
				bossFightStartTimer_ = 0;
				isBossFightStart_ = true;

				fragmentManager_->KillAllFragment();
				fragmentManager_->SetIsPop(false);

				meteoriteManager_->KillAll();
				meteoriteManager_->SetIsPop(false);

				ufoManager_->KillAll();
				ufoManager_->SetIsPop(false);

				ufoBulletManager_->KillAll();

				boss_->RequestIn();

				// BGMストップ
				SUGER::StopWaveLoopSound("GameScene.wav");
				SUGER::PlayWaveLoopSound("Warning.wav", 3);
			}

		}


		// ボス戦固有の処理
		if (isBossFight_) {
			bossBattleframeCount_++;
			if (bossBattleframeCount_ == 60) {
				bossBattleTimer_--;
				bossBattleframeCount_ = 0;
			}

			if (bossBattleTimer_ == 0) {
				gameOverAnimation_ = true;
			}

		}


		// ダメージ破片の更新
		damagePieceManager_->Update();

		// ライトの座標
		light_->GetPunctualLight().pointLight.position = ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_);
		light_->GetPunctualLight().spotLight.direction = Normalize(ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_) - sceneCamera_->GetWorldPos());


		// プレイヤーの更新処理
		player_->Update();

		// ボスの更新処理
		boss_->Update();

		// UFOの弾マネージャの更新
		ufoBulletManager_->Update();

		// 天球の更新
		skydome_->Update();


		// 
		// 地球UI
		// 

		for (uint32_t i = 0; i < 4; i++) {
			earthHPUI_[i]->SetPosition(earthUIPosition_);
			earthHPUI_[i]->SetSize(earthUISize_);
		}

		if (player_->GetHp() <= 25.0f) {
			earthHPUI_[0]->SetIsActive(false);
			earthHPUI_[1]->SetIsActive(false);
			earthHPUI_[2]->SetIsActive(false);
			earthHPUI_[3]->SetIsActive(true);
		} else if (player_->GetHp() <= 50.0f) {
			earthHPUI_[0]->SetIsActive(false);
			earthHPUI_[1]->SetIsActive(false);
			earthHPUI_[2]->SetIsActive(true);
			earthHPUI_[3]->SetIsActive(false);
		} else if (player_->GetHp() <= 75.0f) {
			earthHPUI_[0]->SetIsActive(false);
			earthHPUI_[1]->SetIsActive(true);
			earthHPUI_[2]->SetIsActive(false);
			earthHPUI_[3]->SetIsActive(false);
		} else {
			earthHPUI_[0]->SetIsActive(true);
			earthHPUI_[1]->SetIsActive(false);
			earthHPUI_[2]->SetIsActive(false);
			earthHPUI_[3]->SetIsActive(false);
		}

		if (player_->GetHp() < 10.0f) {
			earthHpNumUI_[1]->SetIsActive(false);
		} else if (player_->GetHp() < 100.0f) {
			earthHpNumUI_[0]->SetIsActive(false);
		}

		// 
		// 地球HPUI
		// 

		// UIのポジションを決定
		earthHpNumUI_[0]->SetPosition(earthHpNumUIPosition_ - Vector2(numGap_, 0.0f));
		earthHpNumUI_[1]->SetPosition(earthHpNumUIPosition_);
		earthHpNumUI_[2]->SetPosition(earthHpNumUIPosition_ + Vector2(numGap_, 0.0f));

		// HPの数字を桁ごとに分割
		earthHpNum_ = SplitDigits(static_cast<uint32_t>(player_->GetHp()));
		// 数字を基に左上をずらして描画
		for (uint32_t i = 0; i < 3; i++) {
			earthHpNumUI_[i]->SetLeftTop(Vector2(earthHpNum_[i] * numberTextureSize_.x, 0.0f));
		}


		// 
		// スコアUI 日数編
		// 

		if (!isBossFightStart_) {
			// 日数によって桁を描画するかどうかの処理
			if (currentDays_ < 10) {
				currentDaysNumUI_[0]->SetIsActive(false);
				currentDaysNumUI_[1]->SetIsActive(false);
				currentDaysNumUI_[2]->SetIsActive(true);
			} else if (currentDays_ < 100) {
				currentDaysNumUI_[0]->SetIsActive(false);
				currentDaysNumUI_[1]->SetIsActive(true);
				currentDaysNumUI_[2]->SetIsActive(true);
			} else {
				currentDaysNumUI_[0]->SetIsActive(true);
				currentDaysNumUI_[1]->SetIsActive(true);
				currentDaysNumUI_[2]->SetIsActive(true);
			}
		} else {
			currentDaysNumUI_[0]->SetIsActive(false);
			currentDaysNumUI_[1]->SetIsActive(false);
			currentDaysNumUI_[2]->SetIsActive(false);
		}

		if (isBossFight_) {
			currentDaysNumUI_[0]->SetIsActive(false);
			currentDaysNumUI_[1]->SetIsActive(false);
			currentDaysNumUI_[2]->SetIsActive(false);
		}

		// ポジションをセット
		currentDaysNumUI_[0]->SetPosition(currentDaysPosition_ - Vector2(numGap_, 0.0f));
		currentDaysNumUI_[1]->SetPosition(currentDaysPosition_);
		currentDaysNumUI_[2]->SetPosition(currentDaysPosition_ + Vector2(numGap_, 0.0f));

		// 数字分割処理
		currentDaysNum_ = SplitDigits(currentDays_);
		// 分割した数字をもとにずらして描画
		for (uint32_t i = 0; i < 3; i++) {
			currentDaysNumUI_[i]->SetLeftTop(Vector2(currentDaysNum_[i] * numberTextureSize_.x, 0.0f));
		}

		//
		// スコアUI 年数編
		//

		//if (!isBossFightStart_) {
		//	// 日数によって桁を描画するかどうかの処理
		//	if (currentYears_ < 10) {
		//		currentYearsNumUI_[0]->SetIsActive(false);
		//		currentYearsNumUI_[1]->SetIsActive(false);
		//		currentYearsNumUI_[2]->SetIsActive(true);
		//	}
		//	else if (currentYears_ < 100) {
		//		currentYearsNumUI_[0]->SetIsActive(false);
		//		currentYearsNumUI_[1]->SetIsActive(true);
		//		currentYearsNumUI_[2]->SetIsActive(true);
		//	}
		//	else {
		//		currentYearsNumUI_[0]->SetIsActive(true);
		//		currentYearsNumUI_[1]->SetIsActive(true);
		//		currentYearsNumUI_[2]->SetIsActive(true);
		//	}
		//}
		//else {
		//	currentYearsNumUI_[0]->SetIsActive(false);
		//	currentYearsNumUI_[1]->SetIsActive(false);
		//	currentYearsNumUI_[2]->SetIsActive(false);
		//}

		if (isBossFight_) {
			currentBossBattleNumUI_[0]->SetIsActive(true);
			currentBossBattleNumUI_[1]->SetIsActive(true);
			currentBossBattleNumUI_[2]->SetIsActive(true);
		}


		// ポジションをセット
		currentBossBattleNumUI_[0]->SetPosition(currentYearsPosition_ - Vector2(numGap_, 0.0f));
		currentBossBattleNumUI_[1]->SetPosition(currentYearsPosition_);
		currentBossBattleNumUI_[2]->SetPosition(currentYearsPosition_ + Vector2(numGap_, 0.0f));

		// 数字分割処理
		currentBossBattleNum_ = SplitDigits(bossBattleTimer_);

		// 分割した数字をもとにずらして描画
		for (uint32_t i = 0; i < 3; i++) {
			currentBossBattleNumUI_[i]->SetLeftTop(Vector2(currentBossBattleNum_[i] * numberTextureSize_.x, 0.0f));
		}

		if (isBossFightStart_) {
			symbolUI_[0]->SetIsActive(false);
			symbolUI_[1]->SetIsActive(false);
		}

		// 
		// ムーン少佐
		// 

		if (currentDays_ > 10 && moonMajarAlpha_ > 0.0f) {
			moonMajarAlpha_ -= 0.05f;
		}

		moonMajarSprite_->SetColor(Vector4(1.0f, 1.0f, 1.0f, moonMajarAlpha_));

		//
		if (moon_->GetIsParent()) {
			buttomUI_->SetLeftTop({ 0,0.0f });
		} else {
			buttomUI_->SetLeftTop({ 0,1.0f * buttomUiSize_.y });
		}

		int fragmentNum = player_->GetScoreData().fragmentNum_ * 100;
		int meteoriteNum = player_->GetScoreData().meteoriteNum_ * 500;
		int ufoNum = player_->GetScoreData().ufoNum_ * 200;
		score_ = fragmentNum + meteoriteNum + ufoNum;

		// ScoreUI
		ActiveUI(scoreUI_, scoreNum_, scorePosition_, score_, numGap2_);

		//
		// コライダーの処理ここから
		//

		// コライダーコンテナをリセット
		SUGER::ClearColliderContainer();

		// 
		// コライダーリスト追加処理
		// 

		SUGER::AddColliderList(player_.get());
		SUGER::AddColliderList(moon_.get());
		boss_->AddColliderList();
		meteoriteManager_->AddColliderList();
		fragmentManager_->AddColliderList();
		bumpManager_->AddColliderList();
		ufoManager_->AddColliderList();
		ufoBulletManager_->AddColliderList();

	}
}

void GameScene::SceneStateFadeOutUpdate() {
	// フェード更新
	fade_->Update();

	if (fade_->IsFinished()) {
		ChangeScene("RESULT");
		fade_->Stop();
	}
}

std::array<int32_t, 3> GameScene::SplitDigits(int32_t number) {
	std::array<int32_t, 3> digits = { 0, 0, 0 };

	if (number > 999) {
		for (uint32_t i = 0; i < 3; i++) {
			digits[i] = 9;
		}
		return digits;
	}

	// 一桁ずつ取り出して配列に格納
	for (int i = 2; i >= 0; --i) {
		digits[i] = number % 10;
		number /= 10;
	}

	return digits;
}

void GameScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize)
{
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}
}

std::array<int32_t, 5> GameScene::SplitDigits5(int32_t number)
{
	std::array<int32_t, 5> digits = { 0, 0, 0,0,0 };

	if (number > 99999) {
		for (uint32_t i = 0; i < 5; i++) {
			digits[i] = 9;
		}
		return digits;
	}

	// 一桁ずつ取り出して配列に格納
	for (int i = 4; i >= 0; --i) {
		digits[i] = number % 10;
		number /= 10;
	}

	return digits;
}

void GameScene::ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<int32_t, 5>& num, Vector2 position, int number, float numGap)
{
	if (number < 10) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(false);
		ui[3]->SetIsActive(false);
		ui[4]->SetIsActive(true);
	}
	else if (number < 100) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(false);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	}
	else if (number < 1000) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(true);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	}
	else if (number < 10000) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(true);
		ui[2]->SetIsActive(true);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	}
	else {
		ui[0]->SetIsActive(true);
		ui[1]->SetIsActive(true);
		ui[2]->SetIsActive(true);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	}

	// ポジションをセット
	ui[0]->SetPosition(position - Vector2(numGap * 4, 0.0f));
	ui[1]->SetPosition(position - Vector2(numGap * 3, 0.0f));
	ui[2]->SetPosition(position - Vector2(numGap * 2, 0.0f));
	ui[3]->SetPosition(position - Vector2(numGap * 1, 0.0f));
	ui[4]->SetPosition(position);

	// 数字分割処理
	num = SplitDigits5(number);
	// 分割した数字をもとにずらして描画
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i]->SetLeftTop(Vector2(num[i] * numberTextureSize2_.x, 0.0f));
	}
}

