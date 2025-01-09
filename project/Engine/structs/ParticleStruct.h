#pragma once
#include "structs/ObjectStructs.h"
#include "enum/EmitterEnum.h"

struct EmitParamater {
	Vector3 position;
	Vector3 rotate;
	Vector3 scale;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
};

struct ParticleData {
	EulerTransform3D transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

// エミッターの設定構造体
struct EmitterSetting {
	// 発生のタイプ
	EmitType emitType = kDefault;
	// 発生場所
	Vector3 minTranslate{};
	Vector3 maxTranslate{};
	// 移動量
	Vector3	minVelocity{};
	Vector3 maxVelocity{};
	// サイズ
	float minScale;
	float maxScale;
	// 色
	Vector3 minColor{};
	Vector3 maxColor{};
	// 生存時間
	float minLifeTime;
	float maxLifeTime;
	// 発生個数
	uint32_t count;
	// 繰り返し発生するかどうか
	bool isRepeat;
	// 発生頻度
	float frequency;
	// 発生速度(radius発生時のパラメータ)
	float speed;
};