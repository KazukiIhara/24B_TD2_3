#pragma once
#include "3d/empty/empty.h"
#include "enum/EmitterEnum.h"
#include "VFX/particle/particle/Particle.h"

class Emitter:public Empty {
public:
	Emitter() = default;
	~Emitter() = default;

	// 初期化
	void Initialize()override;

	// 更新
	void Update()override;

	// パーティクル発生
	virtual void Emit();

	// パーティクルのセット
	void SetParticle(const std::string& particleName);

	// 発生設定初期化
	void InitializeEmitterSetting();

	// 発生数のセット
	void SetCount(const uint32_t& count);
	// 発生頻度のセット
	void SetFrequency(const float& frequency);
	// 繰り返し発生フラグのセット
	void SetIsRepeat(const bool& isRepeat);
	// 発生タイプのセット
	void SetEmitType(const EmitType& emitType);

	// 
	// ランダム発生の閾値設定
	// 

	// エミッター設定のセット
	void SetEmitterSetting(const EmitterSetting&emitterSetting);

	// サイズ最小値
	void SetMinSize(float minSize);
	// サイズ最大値
	void SetMaxSize(float maxSize);
	// 座標最小値
	void SetMinPosition(const Vector3& randomMinPosition);
	// 座標最大値
	void SetMaxPosition(const Vector3& randomMaxPosition);
	// 移動量最小値
	void SetMinVelocity(const Vector3& randomMinVelocity);
	// 移動量最大値
	void SetMaxVelocity(const Vector3& randomMaxVelocity);
	// 色最小値
	void SetMinColor(const Vector3& randomMinColor);
	// 色最大値
	void SetMaxColor(const Vector3& randomMaxColor);
	// 生存時間最小値
	void SetMinLifeTime(float randomMinLifeTime);
	// 生存時間最大値
	void SetMaxLifeTime(float randomMaxLifeTime);

protected:
	// 
	// 基本的な変数
	//

	// デルタタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
	// セットするパーティクル
	Particle* particle_ = nullptr;
	// パーティクル発生設定
	EmitParamater emitParamater_{};
	// エミッターの設定
	EmitterSetting emitterSetting_{};

	//
	// 繰り返し発生用の変数
	//

	// 頻度用時刻
	float frequencyTime_ = 0.0f;
};
