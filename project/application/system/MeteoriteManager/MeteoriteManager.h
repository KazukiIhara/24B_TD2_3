#pragma once

#include "objects/Meteorite/Meteorite.h"

#include <vector>

// 前方宣言
class Earth;
class FragmentManager;

// 隕石マネージャ
class MeteoriteManager {
public:
	void Initialize(Earth* earth,FragmentManager* fragmentManager);

	void Update();

	void AddMeteorite();
private:

	// 
	// 調整パラメータ
	// 

	// データグループ名の定数
	const std::string kParamaterString_ = "MeteoriteParamater";

	// 隕石生成スケール
	Vector3	kMeteritePopScale_ = { 1.0f,1.0f,1.0f };

	// 隕石生成回転量
	Vector3 kMeteritePopRotate_ = { 0.0f,0.0f,0.0f };

	// 隕石生成トランスレート
	Vector3 kMeteritePopTranslate_ = { 3.0f,3.0f,0.0f };

	float speed_ = 0.0f;

	// 隕石リスト
	std::vector<std::unique_ptr<Meteorite>> meteorites_;

	// ひとまず隕石生成地点
	EulerTransform3D meteoritePopTransform_{
		{1.0f,1.0f,1.0f}, // S
		{0.0f,0.0f,0.0f}, // R
		{0.0f,0.0f,0.0f}  // T
	};

private:
	// 地球クラスのインスタンスを受け取る
	Earth* earth_ = nullptr;
	// かけらマネージャのインスタンスを受け取る
	FragmentManager* fragmentManager_ = nullptr;
};