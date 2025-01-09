#pragma once

// C++
#include <cstdint>

// DirectX
#include <d3d12.h>

#include "directX/includes/ComPtr.h"

// 前方宣言
class DXGIManager;

class RTVManager {
public:
	RTVManager() = default;
	~RTVManager() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi);
	// 割り当て関数
	uint32_t Allocate();
	// CPUの特定のインデックスハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandleCPU(uint32_t index);
	// GPUの特定のインデックスハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorHandleGPU(uint32_t index);
	// RTVの作成
	void CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource);

private:
	// DXGIManagerのセット
	void SetDXGI(DXGIManager* dxgi);
public:
	// 最大RTV数
	static const uint32_t kMaxRTVCount = 3;
private:
	// DescriptorHeap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	// rtvDesc
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	// Discriptorのサイズ
	uint32_t descriptorSize_ = 0u;
	// 使用しているviewのインデックス
	uint32_t useIndex = 0;
private:
	// dxgiのインスタンス
	DXGIManager* dxgi_ = nullptr;
};