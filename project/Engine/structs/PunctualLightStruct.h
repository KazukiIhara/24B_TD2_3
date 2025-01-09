#pragma once

#include "math/function/MathFunction.h"

// ディレクショナルライト
struct DirectionalLightForGPU {
	Vector4 color; //ライトの色
	Vector3 direction; //ライトの向き
	float intensity; //輝度
};

// ポイントライト
struct PointLightForGPU {
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
	float padding[2];
};

// スポットライト
struct SpotLightForGPU {
	Vector4 color;
	Vector3 position;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosFalloffStart;
	float cosAngle;
	float padding;
};

// PunctualLight
struct PunctualLightForGPU {
	DirectionalLightForGPU directionalLight;
	PointLightForGPU pointLight;
	SpotLightForGPU spotLight;
};
