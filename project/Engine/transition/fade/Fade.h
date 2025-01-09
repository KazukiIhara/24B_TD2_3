#pragma once

#include <memory>
#include "2d/controller/Object2dController.h"
#include "enum/FadeEnum.h"

class Fade {
public:
	Fade() = default;
	~Fade() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();

	// フェード開始
	void Start(FadeStatus status, float duration);
	// フェードイン更新
	void UpdateFadeIn();
	// フェードアウト更新
	void UpdateFadeOut();
	// フェード終了
	void Stop();
	// フェード終了判定
	bool IsFinished() const;

private:
	// オブジェクトコントローラ
	Object2DController controller_;
	// 現在のフェード状態
	FadeStatus status_ = FadeStatus::None;
	// フェードの持続時間
	float duration_ = 0.0f;
	// 経過時間カウンター
	float counter_ = 0.0f;
	// デルタタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};