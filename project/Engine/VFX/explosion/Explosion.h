#pragma once
#include "3d/empty/empty.h"
#include "VFX/particle/emitterController/EmitterController.h"

class Explosion:public Empty {
public:
	Explosion() = default;
	~Explosion() = default;

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;

	// 発生
	void Emit();


private:
	void ShowEmitterSettingsUI(EmitterSetting& emitterSetting);

private:
	// デルタタイム
	const float kDeltaTime_ = 1.0f / 60.0f;

	// エミッターコントローラ
	EmitterController flashEmitter_;
	EmitterController fireEmitter_;
	EmitterController fireEmitter2_;

	// エミッターの設定
	EmitterSetting flash;
	EmitterSetting fire;
	EmitterSetting fire2;

};

