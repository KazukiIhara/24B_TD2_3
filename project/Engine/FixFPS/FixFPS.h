#pragma once

// C++
#include <chrono>

class FixFPS {
public:
	FixFPS() = default;
	~FixFPS() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();

private:
	// 記録時間
	std::chrono::steady_clock::time_point reference_;
	// 目標FPS
	double targetFPS_ = 60.0;
	// 最初のフレームかどうか
	bool isFirstFrame_ = true;
};