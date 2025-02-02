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

	// プレイフェーズ初期化
	void SceneStatePlayInitialize()override;
	// プレイフェーズ更新
	void SceneStatePlayUpdate()override;

	void InitializeUI(std::array<std::unique_ptr<Object2DController>, 5>& ui,const std::string& name, const std::string& filePath,Vector2 textureSize);

	void UpdateUI();

	std::array<int32_t, 3> SplitDigits3(int32_t number);
	std::array<int32_t, 4> SplitDigits4(int32_t number);
	std::array<int32_t, 5> SplitDigits5(int32_t number);

	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 5>& ui, std::array<int32_t, 5>& num, Vector2 position,int number, float numGap);
	void ActiveUI(std::array<std::unique_ptr<Object2DController>, 3>& ui, std::array<int32_t, 3>& num, Vector2 position,int number, float numGap);

private:
	Vector2 numberTextureSize_ = { 128.0f,192.0f };
	float numGap_ = 64.0f;

	// 全体スコア
	std::array<std::unique_ptr<Object2DController>, 5> allScoreUI_;
	std::array<int32_t, 5> allScoreNum_;
	Vector2 allScorePosition_{ 1980.0f / 2 ,820.0f };
	uint32_t totalScore_ = 0;

	// 欠片スコア
	std::array<std::unique_ptr<Object2DController>, 5> fragmentScoreUI_;
	std::array<int32_t, 5> fragmentScoreNum_;
	Vector2 fragmentScorePosition_{ 1980.0f / 2 ,420.0f };
	uint32_t fragmentScore_;

	// 隕石スコア
	std::array<std::unique_ptr<Object2DController>, 5> meteoriteScoreUI_;
	std::array<int32_t, 5> meteoriteScoreNum_;
	Vector2 meteoriteScorePosition_{ 1980.0f / 2 ,520.0f };
	uint32_t meteoriteScore_;

	// UFOスコア
	std::array<std::unique_ptr<Object2DController>, 5> ufoScoreUI_;
	std::array<int32_t, 5> ufoScoreNum_;
	Vector2 ufoScorePosition_{ 1980.0f / 2 ,620.0f };
	uint32_t ufoScore_;

	// UFOスコア
	std::array<std::unique_ptr<Object2DController>, 5> bossScoreUI_;
	std::array<int32_t, 5> bossScoreNum_;
	Vector2 bossScorePosition_{ 1980.0f / 2 ,720.0f };
	uint32_t bossScore_;


};