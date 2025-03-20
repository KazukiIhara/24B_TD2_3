#include "ResultScene.h"

#include "framework/SUGER.h"

#include "random/Random.h"

void ResultScene::Initialize() {
	// シーンの初期化(初期化処理の先頭)
	BaseScene::Initialize();

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(SUGER::CreateEntity("Skydome", "Skydome"));
	skydome_->SetEnableLight(false);
	skydome_->SetScale(100.0f);
	skydome_->GetUVTransform().scale = { 10.0f,10.0f };

	float xPos = (static_cast<float>(1920) / 5) + 200;

	resultFragment_ = std::make_unique<Object2DController>();
	resultFragment_->Initialize(SUGER::Create2DObject("resultF", "ResultText/ObjectIcoms_x128y128.png"));
	resultFragment_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultFragment_->SetCutOutSize(iconTextureSize_);
	resultFragment_->SetSize(iconTextureSize_ * 0.75f);
	resultFragment_->SetPosition({ xPos,300.0f + (y * 0.0f) });
	resultFragment_->SetLeftTop(Vector2(0 * iconTextureSize_.x, 0.0f));

	resultMeteorite_ = std::make_unique<Object2DController>();
	resultMeteorite_->Initialize(SUGER::Create2DObject("resultM", "ResultText/ObjectIcoms_x128y128.png"));
	resultMeteorite_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultMeteorite_->SetCutOutSize(iconTextureSize_);
	resultMeteorite_->SetSize(iconTextureSize_ * 0.75f);
	resultMeteorite_->SetPosition({ xPos,300.0f + (y * 1.0f) });
	resultMeteorite_->SetLeftTop(Vector2(1 * iconTextureSize_.x, 0.0f));


	resultUfo_ = std::make_unique<Object2DController>();
	resultUfo_->Initialize(SUGER::Create2DObject("resultU", "ResultText/ObjectIcoms_x128y128.png"));
	resultUfo_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultUfo_->SetCutOutSize(iconTextureSize_);
	resultUfo_->SetSize(iconTextureSize_ * 0.75f);
	resultUfo_->SetPosition({ xPos,300.0f + (y * 2.0f) });
	resultUfo_->SetLeftTop(Vector2(2 * iconTextureSize_.x, 0.0f));

	// バックグラウンド
	resultBg_ = std::make_unique<Object2DController>();
	resultBg_->Initialize(SUGER::Create2DObject("0_resultBg", "ResultText/ResultBg.png"));

	// リザルト(テキスト)
	result_ = std::make_unique<Object2DController>();
	result_->Initialize(SUGER::Create2DObject("1_result", "ResultText/Result.png"));
	result_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
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
	resultDay_->SetPosition(Vector2((1920 / 2) - 100, 300.0f + (y * 3.0f)));

	// スコア(テキスト)
	resultScore_ = std::make_unique<Object2DController>();
	resultScore_->Initialize(SUGER::Create2DObject("1_result", "ResultText/ResultScore.png"));
	resultScore_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	resultScore_->SetPosition(Vector2(1920 * 2 / 5, 700));


	// ムーン少佐
	moonMajar_ = std::make_unique<Object2DController>();
	moonMajar_->Initialize(SUGER::Create2DObject("3_major", "MoonMajorText/MajorRank_x1025y192.png"));
	moonMajar_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	moonMajar_->SetSize(moonMajarTextCutSize_);
	moonMajar_->SetCutOutSize(moonMajarTextCutSize_);
	moonMajar_->SetPosition(moonMajarTextPosition_);
	moonMajar_->SetIsActive(false);

	// イコール
	InitializeUI(resultEqual_, "equalUI", "ResultText/ResultSymbol_x128y96.png", symbolTextureSize_,{});

	for (int i = 0; i < resultEqual_.size(); i++) {
		resultEqual_[i]->SetLeftTop({ symbolTextureSize_.x,0 });
	}
	resultEqual_[0]->SetPosition({ equalXpos ,fragmentScorePosition_.y });
	resultEqual_[1]->SetPosition({ equalXpos ,meteoriteScorePosition_.y });
	resultEqual_[2]->SetPosition({ equalXpos ,ufoScorePosition_.y });
	// かける
	InitializeUI(resultMultiplication_, "equalUI", "ResultText/ResultSymbol_x128y96.png", symbolTextureSize_,{});

	resultMultiplication_[0]->SetPosition({ multiplicationXpos ,fragmentScorePosition_.y });
	resultMultiplication_[1]->SetPosition({ multiplicationXpos ,meteoriteScorePosition_.y });
	resultMultiplication_[2]->SetPosition({ multiplicationXpos ,ufoScorePosition_.y });
	resultMultiplication_[3]->SetPosition({ 1920 / 2,300.0f + (y * 4.0f) });
	resultMultiplication_[3]->SetIsActive(true);


	// 全体スコア
	InitializeUI(allScoreUI_, "AllScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,allScorePosition_);

	// スコア
	InitializeUI(scoreUI_, "score", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,scorePosition_);

	
	
	// 欠片スコア
	InitializeUI(fragmentScoreUI_, "fragmentScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,fragmentScorePosition_);

	// 隕石スコア
	InitializeUI(meteoriteScoreUI_, "meteoriteScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,meteoriteScorePosition_);

	// UFOスコア
	InitializeUI(ufoScoreUI_, "ufoScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,ufoScorePosition_);




	// 欠片スコア
	InitializeUI(kFragmentScoreUI_, "kfragmentScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,kFragmentScorePosition_);

	// 隕石スコア
	InitializeUI(kMeteoriteScoreUI_, "kmeteoriteScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,kMeteoriteScorePosition_);

	// UFOスコア
	InitializeUI(kUfoScoreUI_, "kufoScore", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,kUfoScorePosition_);


	// 欠片数
	InitializeUI(fragmentNumUI_, "fragmentNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,fragmentNumPosition_);

	// 隕石数
	InitializeUI(meteoriteNumUI_, "meteoriteNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,meteoriteNumPosition_);

	// UFO数
	InitializeUI(ufoNumUI_, "ufoNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,ufoNumPosition_);

	// 耐えた日数
	InitializeUI(dayUI_, "dayNum", "ResultText/ResultNumber_x48y96.png", numberTextureSize_,dayPosition_);


	// 倍率(少数)
	decimalPointUI_ = std::make_unique<Object2DController>();
	decimalPointUI_->Initialize(SUGER::Create2DObject("decimalPoint", "ResultText/ResultNumber_x48y96.png"));
	decimalPointUI_->SetCutOutSize(numberTextureSize_);
	decimalPointUI_->SetSize(numberTextureSize_);
	decimalPointUI_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	decimalPointUI_->SetPosition(decimalPointPosition_);
	// 倍率(整数)
	integerUI_ = std::make_unique<Object2DController>();
	integerUI_->Initialize(SUGER::Create2DObject("decimalPoint", "ResultText/ResultNumber_x48y96.png"));
	integerUI_->SetCutOutSize(numberTextureSize_);
	integerUI_->SetSize(numberTextureSize_);
	integerUI_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	integerUI_->SetPosition(integerPosition_);


	pointUI_ = std::make_unique<Object2DController>();
	pointUI_->Initialize(SUGER::Create2DObject("decimalPoint", "ResultText/ResultSymbol_x128y96.png"));
	pointUI_->SetCutOutSize(symbolTextureSize_);
	pointUI_->SetSize(symbolTextureSize_);
	pointUI_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	pointUI_->SetPosition(pointPosition_);
	pointUI_->SetLeftTop({ symbolTextureSize_.x * 2.0f,0.0f });

	baiUI_ = std::make_unique<Object2DController>();
	baiUI_->Initialize(SUGER::Create2DObject("decimalPoint", "ResultText/ResultSymbol_x128y96.png"));
	baiUI_->SetCutOutSize(symbolTextureSize_);
	baiUI_->SetSize(symbolTextureSize_);
	baiUI_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	baiUI_->SetPosition(baiosition_);
	baiUI_->SetLeftTop({ symbolTextureSize_.x * 3.0f,0.0f });

	bossUI_ = std::make_unique<Object2DController>();
	bossUI_->Initialize(SUGER::Create2DObject("decimalPoint", "ResultText/ObjectIcoms_x128y128.png"));
	bossUI_->SetCutOutSize(iconTextureSize_);
	bossUI_->SetSize(iconTextureSize_ * 0.75f);
	bossUI_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	bossUI_->SetPosition(bossPoition_);
	bossUI_->SetLeftTop({ iconTextureSize_.x * 3.0f,0.0f });
	bossUI_->SetIsActive(false);
}

void ResultScene::Finalize() {
	// BGMの停止
	SUGER::StopWaveLoopSound("Result.wav");
}


void ResultScene::Operation() {
	if (SUGER::IsGamepadConnected(0)) {
		// パッド操作
		direction_ = static_cast<float>(SUGER::GetLeftStickX(0));
		if (direction_ > 0 && moveScene_ == 0) {
			moveScene_ = 1;
			SUGER::PlayWaveSound("Selection.wav");
		} else if (direction_ < 0 && moveScene_ == 1) {
			moveScene_ = 0;
			SUGER::PlayWaveSound("Selection.wav");
		}
	}
	if (SUGER::PushKey(DIK_A)) {
		moveScene_ = 0;
		SUGER::PlayWaveSound("Selection.wav");
	}

	if (SUGER::PushKey(DIK_D)) {
		moveScene_ = 1;
		SUGER::PlayWaveSound("Selection.wav");
	}

	if (moveScene_ == 0) {
		resultRetry_->SetColor({ 1,1,0,1 });
		resultTitle_->SetColor({ 1,1,1,1 });


		if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
			SUGER::PlayWaveSound("Decision.wav");
			ChangeScene("GAME");
		}
	} else {
		resultRetry_->SetColor({ 1,1,1,1 });
		resultTitle_->SetColor({ 1,1,0,1 });


		if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
			SUGER::PlayWaveSound("Decision.wav");
			ChangeScene("TITLE");
		}
	}
}

void ResultScene::SceneStatePlayInitialize() {
	// BGMの再生
	SUGER::PlayWaveLoopSound("Result.wav");
}

void ResultScene::SceneStatePlayUpdate() {

#ifdef _DEBUG
	ImGui::Begin("Result");
	ImGui::Text("%u Years %u Days", GetGameData().years_, GetGameData().days_);
	ImGui::Text("fragment %u Score %d", GetGameData().fragmentNum_, fragmentScore_);
	ImGui::Text("meteorite %u Score %d", GetGameData().meteoriteNum_, meteoriteScore_);
	ImGui::Text("ufo %u Score %d", GetGameData().ufoNum_, ufoScore_);
	ImGui::Text("AllScore = (score * (1.0f + years + (days / 365))) * if(bossDeath){2}");
	ImGui::Text("Score %d", GetGameData().totalScore_);
	ImGui::End();

#endif // _DEBUG




	fragmentScore_ = GetGameData().fragmentNum_ * 100;
	fragmentNum_ = GetGameData().fragmentNum_;
	meteoriteScore_ = GetGameData().meteoriteNum_ * 500;
	meteoriteNum_ = GetGameData().meteoriteNum_;
	ufoScore_ = GetGameData().ufoNum_ * 200;
	ufoNum_ = GetGameData().ufoNum_;


	score_ = fragmentScore_ + meteoriteScore_ + ufoScore_;
	dayNum_ = uint32_t((float(GetGameData().years_) * 365) + float(GetGameData().days_));
	if (GetGameData().bossDeath_) {
		bossUI_->SetIsActive(true);
		magnificationNum_ = (1.0f + float(GetGameData().years_) + float(float(GetGameData().days_) / float(365))) * 2;
	} else {
		bossUI_->SetIsActive(false);
		magnificationNum_ = (1.0f + float(GetGameData().years_) + float(float(GetGameData().days_) / float(365)));
	}
	SplitDecimalInteger(magnificationNum_, decimalPointNum_, integerNum_);
	magnificationNum_ = formatNumber(magnificationNum_);

	float aa = static_cast<float>(score_);
	float bb = aa * magnificationNum_;
	totalScore_ = static_cast<int>(bb);

	decimalPointUI_->SetLeftTop({ numberTextureSize_.x * decimalPointNum_ ,0 });
	integerUI_->SetLeftTop({ numberTextureSize_.x * integerNum_,0 });

	// UI関係更新
	UpdateUI();


	if (totalScore_ > 45000) {
		currentPage_ = 0;
	} else if (totalScore_ > 30000) {
		currentPage_ = 1;
	} else if (totalScore_ > 10000) {
		currentPage_ = 2;
	} else {
		currentPage_ = 3;
	}


	moonMajar_->SetIsActive(true);
	moonMajar_->SetLeftTop(Vector2(0.0f, static_cast<float>(currentPage_) * moonMajarTextCutSize_.y));

	Operation();

	// 天球の更新
	skydome_->Update();
}

void ResultScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize, Vector2 pos) {
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
		ui[i]->SetIsActive(true);
		ui[i]->SetPosition(pos);

	}
}
void ResultScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 4>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize, Vector2 pos) {
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
		ui[i]->SetIsActive(true);
		ui[i]->SetPosition(pos);

	}
}
void ResultScene::InitializeUI(std::array<std::unique_ptr<Object2DController>, 3>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize, Vector2 pos) {
	for (uint32_t i = 0; i < ui.size(); i++) {
		ui[i] = std::make_unique<Object2DController>();
		ui[i]->Initialize(SUGER::Create2DObject(name, filePath));
		ui[i]->SetCutOutSize(textureSize);
		ui[i]->SetSize(textureSize);
		ui[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
		ui[i]->SetIsActive(true);
		ui[i]->SetPosition(pos);

	}
}

void ResultScene::UpdateUI() {
	
	
	
	
	if (++scoreTimer_ >= MaxScoreTimer_) {
		scoreTimer_ = 0;
		isScore_ = true;
	}

	if (isScore_) {
		// 全体スコア
		ActiveUI(allScoreUI_, allScoreNum_, allScorePosition_, totalScore_, numGap_);

		ActiveUI(scoreUI_, scoreNum_, scorePosition_, score_, numGap_);


		// 欠片スコア
		ActiveUI(fragmentScoreUI_, fragmentScoreNum_, fragmentScorePosition_, fragmentScore_, numGap_);

		// 隕石スコア
		ActiveUI(meteoriteScoreUI_, meteoriteScoreNum_, meteoriteScorePosition_, meteoriteScore_, numGap_);

		// UFOスコア
		ActiveUI(ufoScoreUI_, ufoScoreNum_, ufoScorePosition_, ufoScore_, numGap_);
	}
	else {
		scoreCount_ = Random::GenerateUint32_t(0 ,99999);
		scoreCountA_ = Random::GenerateUint32_t(0 ,99999);
		scoreCountF_ = Random::GenerateUint32_t(0 ,99999);
		scoreCountM_ = Random::GenerateUint32_t(0 ,99999);
		scoreCountU_ = Random::GenerateUint32_t(0 ,99999);
		
		// 全体スコア
		ActiveUI(allScoreUI_, allScoreNum_, allScorePosition_, scoreCountA_, numGap_);

		ActiveUI(scoreUI_, scoreNum_, scorePosition_, scoreCount_, numGap_);


		// 欠片スコア
		ActiveUI(fragmentScoreUI_, fragmentScoreNum_, fragmentScorePosition_, scoreCountF_, numGap_);

		// 隕石スコア
		ActiveUI(meteoriteScoreUI_, meteoriteScoreNum_, meteoriteScorePosition_, scoreCountM_, numGap_);

		// UFOスコア
		ActiveUI(ufoScoreUI_, ufoScoreNum_, ufoScorePosition_, scoreCountU_, numGap_);
	}



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
	ActiveUI(ufoNumUI_, ufoNumNum_, ufoNumPosition_, ufoNum_, numGap_);

	// 耐えた日数
	ActiveUI(dayUI_, dayNumNum_, dayPosition_, dayNum_, numGap_);

	// 倍率少数
	//ActiveUI(decimalPointUI_, decimalPointNumNum_, decimalPointPosition_, decimalPointNum_, numGap_);

}

#pragma region Digits


std::array<int32_t, 3> ResultScene::SplitDigits3(int32_t number) {
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

std::array<int32_t, 4> ResultScene::SplitDigits4(int32_t number) {
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

std::array<int32_t, 5> ResultScene::SplitDigits5(int32_t number) {
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

std::array<float, 5> ResultScene::SplitDigits5(float number) {
	std::array<float, 5> digits = { 0, 0, 0, 0, 0 };

	// 整数部分と小数部分に分ける
	int integerPart = static_cast<int>(number);
	float decimalPart = number - integerPart;

	// 整数部分を配列に格納
	for (int i = 0; i < 5; ++i) {
		digits[i] = float(integerPart % 10);
		integerPart /= 10;
	}

	// 小数部分を小数点以下5桁までに拡大して整数化
	int decimalDigits = static_cast<int>(decimalPart * 100000);

	// 小数部分を配列に格納（空いている位置に格納）
	for (int i = 4; i >= 0 && decimalDigits > 0; --i) {
		if (digits[i] == 0 && i != 0) {  // 整数部分がある位置を飛ばす
			digits[i] = float(decimalDigits % 10);
			decimalDigits /= 10;
		}
	}

	return digits;
}


void ResultScene::SplitDecimalInteger(float num, int32_t& Decimal, int32_t& integer) {
	// 整数部分を1桁に丸めて取得
	integer = static_cast<int32_t>(num) % 10;

	// 小数部分を1桁に丸めて取得
	float decimalPart = num - static_cast<int32_t>(num);
	Decimal = static_cast<int32_t>(decimalPart * 10) % 10;

	num = integer + Decimal / 10.0f;
}

float ResultScene::formatNumber(float value) {
	if (value == 0.0f) return 0.0f; // 特例: 0 の場合はそのまま

	bool isNegative = (value < 0); // 符号を保持
	value = fabs(value); // 絶対値に変換

	// 最上位桁を取得
	int highestDigit = static_cast<int>(value / powf(10, floorf(log10(value))));

	// 小数点以下1桁までに丸める
	float result = highestDigit + (floorf((value - highestDigit * powf(10, floorf(log10(value)))) * 10) / 10);

	return isNegative ? -result : result; // 元の符号を戻す
}





#pragma endregion // Digits

#pragma region UI


void ResultScene::ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<int32_t, 5>& num, Vector2 position, int number, float numGap) {
	if (number < 10) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(false);
		ui[3]->SetIsActive(false);
		ui[4]->SetIsActive(true);
	} else if (number < 100) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(false);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	} else if (number < 1000) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(true);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	} else if (number < 10000) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(true);
		ui[2]->SetIsActive(true);
		ui[3]->SetIsActive(true);
		ui[4]->SetIsActive(true);
	} else {
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
void ResultScene::ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<float, 5>& num, Vector2 position, float number, float numGap) {
	/*if (number < 10) {
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
	else {*/
	ui[0]->SetIsActive(true);
	ui[1]->SetIsActive(true);
	ui[2]->SetIsActive(true);
	ui[3]->SetIsActive(true);
	ui[4]->SetIsActive(true);
	//}

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

void ResultScene::ActiveUI(std::array<std::unique_ptr<Object2DController>, 3>& ui, std::array<int32_t, 3>& num, Vector2 position, int number, float numGap) {
	if (number < 10) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(false);
		ui[2]->SetIsActive(true);
	} else if (number < 100) {
		ui[0]->SetIsActive(false);
		ui[1]->SetIsActive(true);
		ui[2]->SetIsActive(true);
	} else {
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
