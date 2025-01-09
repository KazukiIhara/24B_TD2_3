#pragma once

#include "VFX/particle/emitter/Emitter.h"

class EmitterController {
public:
	EmitterController() = default;
	~EmitterController() = default;

	// 初期化
	void Initialize(const std::string& name);

	// パーティクル発生
	void Emit();

	//
	// Setter
	//

	// スケールのセット
	void SetScale(const Vector3& scale);
	void SetScale(const float& scale);
	// 回転のセット
	void SetRotate(const Vector3& rotate);
	// トランスレートのセット
	void SetTranslate(const Vector3& translate);

	// 親トランスフォームのセット
	void SetParent(WorldTransform* parent);

	// パーティクルのセット
	void SetParticle(const std::string& particleName);

	// 同時発生個数のセット
	void SetCount(const uint32_t& count);

	// 繰り返し発生フラグセット
	void SetIsRepeat(const bool& isRepeat);

	// 発生頻度のセット
	void SetFrequency(const float& frequency);

	// 発生タイプのセット
	void SetEmitType(const EmitType& emitType);


	// エミッターの設定のセット
	void SetEmitterSetting(const EmitterSetting& emitterSetting);

	// 発生場所のランダム閾値
	void SetMinPosition(const Vector3& minPosition);
	void SetMaxPosition(const Vector3& maxPosition);

	// サイズのランダム閾値
	void SetMinSize(float minSize);
	void SetMaxSize(float maxSize);

	// 移動量のランダム閾値
	void SetMinVelocity(const Vector3& minVelocity);
	void SetMaxVelocity(const Vector3& maxVelocity);

	// 色のランダム閾値
	void SetMinColor(const Vector3& minColor);
	void SetMaxColor(const Vector3& maxColor);

	// 生存時間のランダム閾値
	void SetMinLifeTime(float minLifeTime);
	void SetMaxLifeTime(float maxLifeTime);

	//
	// Getter
	//

	// スケールの取得
	const Vector3& GetScale()const;
	// 回転の取得
	const Vector3& GetRotate()const;
	// 移動量の取得
	const Vector3& GetTranslate()const;

	// ワールド座標のゲッター
	Vector3 GetWorldPosition()const;

	// ワールドトランスフォームのポインタの取得
	WorldTransform* GetWorldTransformPtr();

private:
	// エミッター
	Emitter* emitter_ = nullptr;
};