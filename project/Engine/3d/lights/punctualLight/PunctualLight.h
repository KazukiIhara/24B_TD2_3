#pragma once
#pragma once

// DirectX
#include <d3d12.h>

// MyHedder
#include "structs/PunctualLightStruct.h"
#include "directX/includes/ComPtr.h"

class PunctualLight {
public: // メンバ関数
	// 初期化
	void Initialize();
	void Update();
	void TransferLight();

	void InitializeLightSetting();
	void SetPunctualLightSetting(const PunctualLightForGPU& punctualLightSetting);

private: // 非公開メンバ関数
	void CreatePunctualLightResource();
	void MapPunctualLightData();

	void UpdateDirectionalLight();
	void UpdatePointLight();
	void UpdateSpotLight();

private: // メンバ変数
	// PunctualLight用のリソース
	ComPtr<ID3D12Resource> punctualLightResource_ = nullptr;
	// シェーダーに送るライトのデータ
	PunctualLightForGPU* punctualLightData_ = nullptr;
	// PunctualLight
	PunctualLightForGPU punctualLight{};

};
