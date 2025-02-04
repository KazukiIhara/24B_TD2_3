#include "ResultScene.h"

#include "framework/SUGER.h"

void ResultScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// バックグラウンド
	resultBg_ = std::make_unique<Object2DController>();
	resultBg_->Initialize(SUGER::Create2DObject("0_resultBg", "ResultText/ResultBg.png"));

	// リザルト(テキスト)
	result_ = std::make_unique<Object2DController>();
	result_->Initialize(SUGER::Create2DObject("1_result", "ResultText/Result.png"));
	result_->SetAnchorPoint(Vector2{ 0.5f,0.5f});
	result_->SetPosition(Vector2(1920 / 2, 200));
	
	// リトライ(テキスト)
	resultRetry_ = std::make_unique<Object2DController>();
	resultRetry_->Initialize(SUGER::Create2DObject("1_result", "ResultText/ResultRetry.png"));
	resultRetry_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultRetry_->SetPosition(Vector2(1920 / 3, 800));

	// タイトル(テキスト)
	resultTitle_ = std::make_unique<Object2DController>();
	resultTitle_->Initialize(SUGER::Create2DObject("1_result", "ResultText/ResultTitle.png"));
	resultTitle_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultTitle_->SetPosition(Vector2(1920 * 2 / 3, 800));

	 // 耐えた日(テキスト)
	resultDay_ = std::make_unique<Object2DController>();
	resultDay_->Initialize(SUGER::Create2DObject("1_result", "ResultText/ResultDay.png"));
	resultDay_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultDay_->SetPosition(Vector2((1920 / 2) -100, 300.0f + (y * 3.0f)));

	// スコア(テキスト)
	resultScore_ = std::make_unique<Object2DController>();
	resultScore_->Initialize(SUGER::Create2DObject("1_result", "ResultText/ResultScore.png"));
	resultScore_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultScore_->SetPosition(Vector2(1920 * 2 / 5, 700));

	// イコール
	InitializeUI(resultEqual_,"equalUI","ResultText/ResultSymbol_x128y96.png", symbolTextureSize_);
	
	for (int i = 0; i < resultEqual_.size(); i++) {
		resultEqual_[i]->SetLeftTop({ symbolTextureSize_.x,0});
	}
	resultEqual_[0]->SetPosition({ equalXpos ,fragmentScorePosition_.y });
	resultEqual_[1]->SetPosition({ equalXpos ,meteoriteScorePosition_.y });
	resultEqual_[2]->SetPosition({ equalXpos ,ufoScorePosition_.y });
	// かける
	InitializeUI(resultMultiplication_,"equalUI","ResultText/ResultSymbol_x128y96.png", symbolTextureSize_);
	
	resultMultiplication_[0]->SetPosition({ multiplicationXpos ,fragmentScorePosition_.y });
	resultMultiplication_[1]->SetPosition({ multiplicationXpos ,meteoriteScorePosition_.y });
	resultMultiplication_[2]->SetPosition({ multiplicationXpos ,ufoScorePosition_.y });
	resultMultiplication_[3]->SetPosition({ 1920 / 2,300.0f + (y * 4.0f) });

	// 全体スコア
	InitializeUI(allScoreUI_, "AllScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// 欠片スコア
	InitializeUI(fragmentScoreUI_, "fragmentScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// 隕石スコア
	InitializeUI(meteoriteScoreUI_, "meteoriteScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// UFOスコア
	InitializeUI(ufoScoreUI_, "ufoScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	
	// 欠片スコア
	InitializeUI(kFragmentScoreUI_, "kfragmentScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// 隕石スコア
	InitializeUI(kMeteoriteScoreUI_, "kmeteoriteScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// UFOスコア
	InitializeUI(kUfoScoreUI_, "kufoScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);


	// 欠片数
	InitializeUI(fragmentNumUI_, "fragmentNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// 隕石数
	InitializeUI(meteoriteNumUI_, "meteoriteNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// UFO数
	InitializeUI(ufoNumUI_, "ufoNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	// 耐えた日数
	InitializeUI(dayUI_, "dayNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_);

	
	

}

void ResultScene::Finalize() {}

void ResultScene::SceneStatePlayInitialize() {}

void ResultScene::SceneStatePlayUpdate() {
	


	ImGui::Begin("Result");

	ImGui::Text("%u Years %u Days", GetGameData().years_, GetGameData().days_);
	fragmentScore_ = GetGameData().fragmentNum_ * 100;
	fragmentNum_ = GetGameData().fragmentNum_;
	ImGui::Text("fragment %u Score %d", GetGameData().fragmentNum_, fragmentScore_);
	meteoriteScore_ = GetGameData().meteoriteNum_ * 500;
	meteoriteNum_ = GetGameData().meteoriteNum_;
	ImGui::Text("meteorite %u Score %d", GetGameData().meteoriteNum_, meteoriteScore_);
	ufoScore_ = GetGameData().ufoNum_ * 200;
	ufoScore_ = GetGameData().ufoNum_;
	ImGui::Text("ufo %u Score %d", GetGameData().ufoNum_, ufoScore_);
	int score = fragmentScore_ + meteoriteScore_ + ufoScore_;
	int allScore = int(float(score) * (1.0f + float(GetGameData().years_) + float(float(GetGameData().days_) / float(365))));
	
	dayNum_ = uint32_t((float(GetGameData().years_) * 365) + float(GetGameData().days_));
	
	if (GetGameData().bossDeath_) {
		GetGameData().totalScore_ = allScore * 2;
	}
	else {
		GetGameData().totalScore_ = allScore;
	}
	totalScore_ = GetGameData().totalScore_;
	ImGui::Text("AllScore = (score * (1.0f + years + (days / 365))) * if(bossDeath){2}");
	ImGui::Text("Score %d", GetGameData().totalScore_);
	ImGui::End();


	// UI関係更新
	UpdateUI();

	

	

	if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
		ChangeScene("TITLE");
	}

}

void ResultScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 5>& ui,const std::string& name, const std::string& filePath, Vector2 textureSize)
{
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}
}
void ResultScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 4>& ui,const std::string& name, const std::string& filePath, Vector2 textureSize)
{
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}
}
void ResultScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 3>& ui,const std::string& name, const std::string& filePath, Vector2 textureSize)
{
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}
}

void ResultScene::UpdateUI()
{
	// 全体スコア
	ActiveUI(allScoreUI_, allScoreNum_, allScorePosition_, totalScore_, numGap_);

	// 欠片スコア
	ActiveUI(fragmentScoreUI_, fragmentScoreNum_, fragmentScorePosition_, fragmentScore_, numGap_);

	// 隕石スコア
	ActiveUI(meteoriteScoreUI_, meteoriteScoreNum_, meteoriteScorePosition_, meteoriteScore_, numGap_);

	// UFOスコア
	ActiveUI(ufoScoreUI_, ufoScoreNum_, ufoScorePosition_, ufoScore_, numGap_);
	
	

	
	// 欠片スコア
	ActiveUI(kFragmentScoreUI_, kFragmentScoreNum_, kFragmentScorePosition_, kFragmentScore_, numGap_);

	// 隕石スコア
	ActiveUI(kMeteoriteScoreUI_, kMeteoriteScoreNum_, kMeteoriteScorePosition_, kMeteoriteScore_, numGap_);

	// UFOスコア
	ActiveUI(kUfoScoreUI_, kUfoScoreNum_, kUfoScorePosition_, kUfoScore_, numGap_);
	

	// 欠片数
	ActiveUI(fragmentNumUI_, fragmentNumNum_, fragmentNumPosition_, fragmentNum_, numGap_);

	// 隕石数
	ActiveUI(meteoriteNumUI_, meteoriteNumNum_, meteoriteNumPosition_, meteoriteNum_, numGap_);

	// UFO数
	ActiveUI(ufoNumUI_, ufoNumNum_, ufoNumPosition_,ufoNum_ , numGap_);
	
	// 耐えた日数
	ActiveUI(dayUI_, dayNumNum_, dayPosition_,dayNum_ , numGap_);

}

#pragma region Digits


std::array<int32_t, 3> ResultScene::SplitDigits3(int32_t number)
{
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

std::array<int32_t, 4> ResultScene::SplitDigits4(int32_t number)
{
	std::array<int32_t, 4> digits = { 0, 0, 0,0 };

	if (number > 9999) {
		for (uint32_t i = 0; i < 4; i++) {
			digits[i] = 9;
		}
		return digits;
	}

	// 一桁ずつ取り出して配列に格納
	for (int i = 3; i >= 0; --i) {
		digits[i] = number % 10;
		number /= 10;
	}

	return digits;
}

std::array<int32_t, 5> ResultScene::SplitDigits5(int32_t number)
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

#pragma endregion // Digits

#pragma region UI


void ResultScene::ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<int32_t, 5>& num, Vector2 position, int number, float numGap)
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
		ui[i]->SetLeftTop(Vector2(num[i] * numberTextureSize_.x, 0.0f));
	}
}

void ResultScene::ActiveUI(std::array<std::unique_ptr<Object2DController>, 3>& ui, std::array<int32_t, 3>& num, Vector2 position, int number, float numGap)
{
	if (number < 10) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(true);
	}
	else if (number < 100) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(true);
		ui[2]->SetIsActive(true);
	}
	else {
		ui[0]->SetIsActive(true);
		ui[1]->SetIsActive(true);
		ui[2]->SetIsActive(true);
	}

	// ポジションをセット
	ui[0]->SetPosition(position - Vector2(numGap * 2, 0.0f));
	ui[1]->SetPosition(position - Vector2(numGap * 1, 0.0f));
	ui[2]->SetPosition(position - Vector2(numGap * 0, 0.0f));

	// 数字分割処理
	num = SplitDigits3(number);
	// 分割した数字をもとにずらして描画
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i]->SetLeftTop(Vector2(num[i] * numberTextureSize_.x, 0.0f));
	}
}

#pragma endregion // UI関係
