#pragma once

// Common
#include "iScene/base/BaseScene.h"
#include "2d/controller/Object2dController.h"
#include "3d/entityController/EntityController.h"
#include "VFX/particle/emitterController/EmitterController.h"
#include "3d/lineController/LineController.h"


class ResultScene :public BaseScene {
public:
	ResultScene() = default;
	~ResultScene() = default;
	// 初期化
	void Initialize()override;
	// 終了
	void Finalize()override;

	// 操作
	void Operation();

	// プレイフェーズ初期化
	void SceneStatePlayInitialize()override;
	// プレイフェーズ更新
	void SceneStatePlayUpdate()override;

	void InitializeUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize);
	void InitializeUI(std::array<std::unique_ptr<Object2DController>, 4>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize);
	void InitializeUI(std::array<std::unique_ptr<Object2DController>, 3>& ui, const std::string& name, const std::string& filePath, Vector2 textureSize);

	void UpdateUI();

	std::array<int32_t, 3> SplitDigits3(int32_t number);
	std::array<int32_t, 4> SplitDigits4(int32_t number);
	std::array<int32_t, 5> SplitDigits5(int32_t number);
	std::array<float, 5> SplitDigits5(float number);

	void SplitDecimalInteger(float num, int32_t& Decimal, int32_t& integer);

	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<int32_t, 5>& num, Vector2 position, int number, float numGap);
	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 4>& ui, std::array<int32_t, 4>& num, Vector2 position, int number, float numGap);
	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<float, 5>& num, Vector2 position, float number, float numGap);
	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 3>& ui, std::array<int32_t, 3>& num, Vector2 position, int number, float numGap);

private:

	// バックグラウンド
	std::unique_ptr<Object2DController> resultBg_;
	// リザルト(テキスト)
	std::unique_ptr<Object2DController> result_;
	// リトライ(テキスト)
	std::unique_ptr<Object2DController> resultRetry_;
	// タイトル(テキスト)
	std::unique_ptr<Object2DController> resultTitle_;
	// 耐えた日(テキスト)
	std::unique_ptr<Object2DController> resultDay_;
	// スコア(テキスト)
	std::unique_ptr<Object2DController> resultScore_;


	Vector2 symbolTextureSize_ = { 128.0f,96.0f };
	// イコール(テキスト)
	std::array<std::unique_ptr<Object2DController>, 3> resultEqual_;
	// ×(テキスト)
	std::array<std::unique_ptr<Object2DController>, 4> resultMultiplication_;


private:
	Vector2 numberTextureSize_ = { 48.0f,96.0f };
	float numGap_ = 48.0f;

	float scoreXpos = (1980.0f * 3 / 5) + 100;
	float kScoreXpos = 1980.0f * 8 / 17;
	float numXpos = 1980.0f * 15 / 40;
	float equalXpos = 1980.0f / 2;
	float multiplicationXpos = 1980.0f * 2 / 5;

	// 全体スコア
	std::array<std::unique_ptr<Object2DController>, 5> allScoreUI_;
	std::array<int32_t, 5> allScoreNum_;
	Vector2 allScorePosition_{ scoreXpos ,700.0f };
	uint32_t totalScore_ = 0;

	float y = 80;

	// 欠片スコア
	std::array<std::unique_ptr<Object2DController>, 5> fragmentScoreUI_;
	std::array<int32_t, 5> fragmentScoreNum_;
	Vector2 fragmentScorePosition_{ scoreXpos,300.0f };
	uint32_t fragmentScore_;

	// 隕石スコア
	std::array<std::unique_ptr<Object2DController>, 5> meteoriteScoreUI_;
	std::array<int32_t, 5> meteoriteScoreNum_;
	Vector2 meteoriteScorePosition_{ scoreXpos ,300.0f + (y * 1.0f) };
	uint32_t meteoriteScore_;

	// UFOスコア
	std::array<std::unique_ptr<Object2DController>, 5> ufoScoreUI_;
	std::array<int32_t, 5> ufoScoreNum_;
	Vector2 ufoScorePosition_{ scoreXpos,300.0f + (y * 2.0f) };
	uint32_t ufoScore_;




	// 欠片スコア
	std::array<std::unique_ptr<Object2DController>, 3> kFragmentScoreUI_;
	std::array<int32_t, 3> kFragmentScoreNum_;
	Vector2 kFragmentScorePosition_{ kScoreXpos,300.0f };
	uint32_t kFragmentScore_ = 100;

	// 隕石スコア
	std::array<std::unique_ptr<Object2DController>, 3> kMeteoriteScoreUI_;
	std::array<int32_t, 3> kMeteoriteScoreNum_;
	Vector2 kMeteoriteScorePosition_{ kScoreXpos ,300.0f + (y * 1.0f) };
	uint32_t kMeteoriteScore_ = 500;

	// UFOスコア
	std::array<std::unique_ptr<Object2DController>, 3> kUfoScoreUI_;
	std::array<int32_t, 3> kUfoScoreNum_;
	Vector2 kUfoScorePosition_{ kScoreXpos,300.0f + (y * 2.0f) };
	uint32_t kUfoScore_ = 200;




	// 欠片数
	std::array<std::unique_ptr<Object2DController>, 5> fragmentNumUI_;
	std::array<int32_t, 5> fragmentNumNum_;
	Vector2 fragmentNumPosition_{ numXpos,300.0f };
	uint32_t fragmentNum_;

	// 隕石数
	std::array<std::unique_ptr<Object2DController>, 5> meteoriteNumUI_;
	std::array<int32_t, 5> meteoriteNumNum_;
	Vector2 meteoriteNumPosition_{ numXpos ,300.0f + (y * 1.0f) };
	uint32_t meteoriteNum_;

	// UFO数
	std::array<std::unique_ptr<Object2DController>, 5> ufoNumUI_;
	std::array<int32_t, 5> ufoNumNum_;
	Vector2 ufoNumPosition_{ numXpos,300.0f + (y * 2.0f) };
	uint32_t ufoNum_;

	// 耐えた日数
	std::array<std::unique_ptr<Object2DController>, 5> dayUI_;
	std::array<int32_t, 5> dayNumNum_;
	Vector2 dayPosition_{ scoreXpos,300.0f + (y * 3.0f) };
	uint32_t dayNum_;

	// 倍率
	std::array<std::unique_ptr<Object2DController>, 5> magnificationUI_;
	std::array<float, 5> magnificationNumNum_;
	Vector2 magnificationPosition_{ scoreXpos,300.0f + (y * 4.0f) };
	float magnificationNum_;

	// 倍率小数点
	std::array<std::unique_ptr<Object2DController>, 4> decimalPointUI_;
	std::array<int32_t, 4> decimalPointNumNum_;
	Vector3 decimalPointPosition_{ scoreXpos,300.0f + (y * 4.0f) };
	int32_t decimalPointNum_;

	// 倍率整数
	std::array<std::unique_ptr<Object2DController>, 4> integerUI_;
	std::array<int32_t, 4> integerNumNum_;
	Vector3 integerPosition_{ scoreXpos + 10 ,300.0f + (y * 4.0f) };
	int32_t integerNum_;
private:
	int moveScene_ = 0;
	float direction_;
};