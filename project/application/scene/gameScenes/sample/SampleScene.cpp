// This
#include "SampleScene.h"

// ImGui
#include "externals/imgui/imgui.h"

// MyHedder
#include "framework/SUGER.h"
#include "debugTools/logger/Logger.h"

void SampleScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// 
	// シーンの初期化処理ここから
	// 

	// レベルデータをシーンにインポート
	levelDataImporter_.Import("baseScene");

	// エンティティの作成とエンティティコントローラ初期化
	// プロ生ちゃん
	pronamaChan_ = std::make_unique<PronamaChan>();
	pronamaChan_->Initialize(SUGER::CreateEntity("pronamaChan", "walk"));
	pronamaChan_->CreateCollider(kNone, kSphere, 1.0f);
	pronamaChan_->SetColliderTranslate(Vector3(0.0f, 1.0f, 0.0f));
	pronamaChan_->SetRotate(Vector3(0.0f, std::numbers::pi_v<float>, 0.0f));
	// ライトを無効化
	pronamaChan_->SetEnableLight(true);

	// ティーポット
	// 初期トランスフォームを設定
	EulerTransform3D teapotDefaultTransform;
	teapotDefaultTransform.rotate = { 0.0f,0.0f,0.0f };
	teapotDefaultTransform.scale = { 1.0f,1.0f,1.0f };
	teapotDefaultTransform.translate = { 3.0f,0.0f,0.0f };
	// teapot作成
	teapot_ = std::make_unique<Teapot>();
	teapot_->Initialize(SUGER::CreateEntity("teapot", "teapot", teapotDefaultTransform));
	teapot_->CreateCollider(kNone, kSphere, 1.0f);

	// オブジェクト2Dの作成とコントローラの初期化
	pronamaChanTex.Initialize(SUGER::Create2DObject("0_pronama_chan", "pronama_chan.png"));

	// オブジェクト2Dコントローラを使ってポジションとアンカーポイントをセット
	pronamaChanTex.SetSize(Vector2(400.0f, 400.0f));
	pronamaChanTex.SetPosition(pronamaChanTex.GetSize() / 2.0f);
	pronamaChanTex.SetAnchorPoint(Vector2(0.5f, 0.5f));

	// 板ポリパーティクルの作成
	SUGER::CreateParticle("sampleParticle", ParticleType::kPlane, "circle.png");
	// エミッターの作成
	SUGER::CreateEmitter("sampleEmitter");

	// エミッターのコントローラを初期化
	emitter_.Initialize("sampleEmitter");
	// エミッターにパーティクルをセット
	emitter_.SetParticle("sampleParticle");
	// エミッターの発生個数を変更
	emitter_.SetCount(10);
	// エミッターの発生タイプを設定
	emitter_.SetEmitType(kRandom);
	// 繰り返し発生オフ
	emitter_.SetIsRepeat(true);

	emitter_.SetMaxVelocity(Vector3(2.0f, 2.0f, 2.0f));
	emitter_.SetMinVelocity(Vector3(-2.0f, -2.0f, -2.0f));

	//
	// GrobalData
	//

	// プロ生ちゃんのデータグループを作成
	SUGER::AddGrobalDataGroup("Pronama_Chan");
	// グローバルデータのプロ生ちゃんグループにトランスレート情報を追加
	SUGER::AddGrobalDataItem("Pronama_Chan", "translate", pronamaChan_->GetTranslate());
	// グローバルデータのプロ生ちゃんグループにRotate情報を追加
	SUGER::AddGrobalDataItem("Pronama_Chan", "rotate", pronamaChan_->GetRotate());
	// グローバルデータのプロ生ちゃんグループにScale情報を追加
	SUGER::AddGrobalDataItem("Pronama_Chan", "scale", pronamaChan_->GetScale());

}

void SampleScene::Finalize() {
	// デバッグ用文字
	Logger::Log("SampleScene,Finalized\n");

}

void SampleScene::SceneStatePlayInitialize() {

}

void SampleScene::SceneStatePlayUpdate() {

	//
	// GrobalData
	//

	// 更新処理の初めにグローバルデータクラスに保存されている値を取得
	pronamaChan_->SetTranslate(SUGER::GetGrobalDataValueVector3("Pronama_Chan", "translate"));
	pronamaChan_->SetRotate(SUGER::GetGrobalDataValueVector3("Pronama_Chan", "rotate"));
	pronamaChan_->SetScale(SUGER::GetGrobalDataValueVector3("Pronama_Chan", "scale"));

	// 
	// シーンの更新処理ここから
	// 

	// Uキーを押すとBGM再生
	if (SUGER::TriggerKey(DIK_U)) {
		SUGER::PlayWaveSound("Alarm01.wav");
	}
	if (SUGER::TriggerKey(DIK_I)) {
		SUGER::StopWaveSound("Alarm01.wav");
	}

	// スペースキーを押すと発生
	if (SUGER::PushKey(DIK_SPACE)) {
		emitter_.Emit();
	}

	if (SUGER::PushKey(DIK_D)) {
		// スプライトを回転
		pronamaChanTex.SetRotation(pronamaChanTex.GetRotation() + 0.01f);
		pronamaChan_->SetTranslate(Vector3(pronamaChan_->GetTranslate().x + 0.1f, pronamaChan_->GetTranslate().y, pronamaChan_->GetTranslate().z));
	} else if (SUGER::PushKey(DIK_A)) {
		// スプライトを回転
		pronamaChanTex.SetRotation(pronamaChanTex.GetRotation() - 0.01f);
		pronamaChan_->SetTranslate(Vector3(pronamaChan_->GetTranslate().x - 0.1f, pronamaChan_->GetTranslate().y, pronamaChan_->GetTranslate().z));
	}

	// 
	// エンティティの更新処理ここから
	// 

	// プロ生ちゃんの更新処理
	pronamaChan_->Update();

	//
	// エンティティの更新処理ここまで
	//

	//
	// シーンの更新処理ここまで
	//

	//
	// コライダーの処理ここから
	//

	// コライダーコンテナをリセット
	SUGER::ClearColliderContainer();

	// コライダーリストに追加していく
	// プロ生ちゃん
	SUGER::AddColliderList(pronamaChan_.get());
	// ティーポット
	SUGER::AddColliderList(teapot_.get());

	//
	// コライダーの処理ここまで
	//

	//
	// GrobalData
	//

	// 行列更新の手前でローカルデータをグローバルデータクラスに挿入
	SUGER::SetGrobalDataValue("Pronama_Chan", "translate", pronamaChan_->GetTranslate());
	SUGER::SetGrobalDataValue("Pronama_Chan", "rotate", pronamaChan_->GetRotate());
	SUGER::SetGrobalDataValue("Pronama_Chan", "scale", pronamaChan_->GetScale());

}