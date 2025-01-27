#include "GameScene.h"

#include "framework/SUGER.h"

#include "random/Random.h"

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
	skydome_->SetScale(100.0f);
	skydome_->GetUVTransform().scale = { 10.0f,10.0f };

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
	fragmentManager_->SetPlayer(player_.get());
	// 
	// 隕石マネージャの初期化処理
	// 

	meteoriteManager_ = std::make_unique<MeteoriteManager>();
	meteoriteManager_->Initialize(earth_.get(), player_.get(), fragmentManager_.get());
	meteoriteManager_->AddMeteorite();

	//
	// たんこぶマネージャーの初期化処理
	//

	bumpManager_ = std::make_unique<BumpManager>();
	bumpManager_->Initialize();
	bumpManager_->SetPlayer(player_.get());
	bumpManager_->SetEarth(earth_.get());
	// 
	player_->SetBumpManager(bumpManager_.get());



	// 板ポリパーティクルの作成
	SUGER::CreateParticle("dustParticle", ParticleType::kPlane, "circle.png");
	SUGER::CreateParticle("fragmentParticle", ParticleType::kModel,"Fragment/Fragment.gltf");


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

	SUGER::AddGrobalDataGroup("UI");
	SUGER::AddGrobalDataItem("UI", "EarthUIPosX", earthUIPosition_.x);
	SUGER::AddGrobalDataItem("UI", "EarthUIPosY", earthUIPosition_.y);

	earthUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosX");
	earthUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosY");

	for (uint32_t i = 0; i < 4; i++) {
		earthHPUI_[i]->SetPosition(earthUIPosition_);
	}

	for (uint32_t i = 0; i < 3; i++) {
		earthHpNumUI_[i] = std::make_unique<Object2DController>();
		earthHpNumUI_[i]->Initialize(SUGER::Create2DObject("0_earthHpNumUI", "Number/Number_x128y192.png"));
		earthHpNumUI_[i]->SetCutOutSize(numberTextureSize_);
		earthHpNumUI_[i]->SetSize(numberTextureSize_ / 2.0f);
	}

	earthHpNum_ = SplitDigits(static_cast<uint32_t>(earth_->GetHp()));

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

	earthUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosX");
	earthUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUIPosY");

	earthHpNumUIPosition_.x = SUGER::GetGrobalDataValueFloat("UI", "EarthUINumPosX");
	earthHpNumUIPosition_.y = SUGER::GetGrobalDataValueFloat("UI", "EarthUINumPosY");

#endif // DEBUG

	if (SUGER::TriggerKey(DIK_R)) {
		ChangeScene("GAME");
	}

	// シェイクテスト用
	if (SUGER::TriggerKey(DIK_SPACE)) {
		sceneCamera_->Shake(15.0f, 0.5f);
	}

	// 地球の更新
	earth_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// たんこぶマネージャーの更新
	bumpManager_->Update();


	// 隕石マネージャの更新
	meteoriteManager_->Update();

	// かけらマネージャの更新
	fragmentManager_->Update();


	// 天球の更新
	skydome_->Update();


	for (uint32_t i = 0; i < 4; i++) {
		earthHPUI_[i]->SetPosition(earthUIPosition_);
	}

	if (earth_->GetHp() <= 75.0f) {
		earthHPUI_[0]->SetIsActive(false);
	}
	if (earth_->GetHp() <= 50.0f) {
		earthHPUI_[1]->SetIsActive(false);
	}
	if (earth_->GetHp() <= 25.0f) {
		earthHPUI_[2]->SetIsActive(false);
	}

	if (earth_->GetHp() < 100.0f) {
		earthHpNumUI_[0]->SetIsActive(false);
	}

	// UIのポジションを決定
	earthHpNumUI_[0]->SetPosition(earthHpNumUIPosition_ - Vector2(numGap_, 0.0f));
	earthHpNumUI_[1]->SetPosition(earthHpNumUIPosition_);
	earthHpNumUI_[2]->SetPosition(earthHpNumUIPosition_ + Vector2(numGap_, 0.0f));

	// HPの数字を桁ごとに分割
	earthHpNum_ = SplitDigits(static_cast<uint32_t>(earth_->GetHp()));
	// 数字を基に左上をずらして描画
	for (uint32_t i = 0; i < 3; i++) {
		earthHpNumUI_[i]->SetLeftTop(Vector2(earthHpNum_[i] * numberTextureSize_.x, 0.0f));
	}

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

