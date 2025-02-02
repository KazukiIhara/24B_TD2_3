#include "ResultScene.h"

#include "framework/SUGER.h"

void ResultScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	InitializeUI(scoreUI_, "AllScore", "Number/Number_x128y192.png", numberTextureSize_);



}

void ResultScene::Finalize() {}

void ResultScene::SceneStatePlayInitialize() {}

void ResultScene::SceneStatePlayUpdate() {
	


	ImGui::Begin("Result");

	ImGui::Text("%u Years %u Days", GetGameData().years_, GetGameData().days_);
	int fragmentScore = GetGameData().fragmentNum_ * 100;
	ImGui::Text("fragment %u Score %d", GetGameData().fragmentNum_, fragmentScore);
	int meteoriteScore = GetGameData().meteoriteNum_ * 500;
	ImGui::Text("meteorite %u Score %d", GetGameData().meteoriteNum_, meteoriteScore);
	int ufoScore = GetGameData().ufoNum_ * 200;
	ImGui::Text("ufo %u Score %d", GetGameData().ufoNum_, ufoScore);
	int bossScore = int(GetGameData().bossDeath_) * 2000;
	ImGui::Text("boss %u Score %d", uint32_t(GetGameData().bossDeath_), bossScore);
	int score = fragmentScore + meteoriteScore + ufoScore + bossScore;
	int allScore = int(float(score) * (1.0f + float(GetGameData().years_) + float(float(GetGameData().days_) / float(365))));
	if (GetGameData().bossDeath_) {
		GetGameData().totalScore_ = allScore * 2;
	}
	else {
		GetGameData().totalScore_ = allScore;
	}
	totalScore_ = GetGameData().totalScore_;
	ImGui::Text("Score %d", GetGameData().totalScore_);
	ImGui::End();


	ActiveUI(scoreUI_, scoreNum_,scorePosition_ ,totalScore_, numGap_);

	

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
		ui[i]->SetSize(textureSize / 2.0f);
	}
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
