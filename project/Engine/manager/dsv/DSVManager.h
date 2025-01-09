#pragma once

// C++
#include <cstdint>

// DirectX
#include <d3d12.h>

#include "directX/includes/ComPtr.h"

// 前方宣言
class DXGIManager;
class DirectXCommand;

class DSVManager {
public:
	DSVManager() = default;
	~DSVManager() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi, DirectXCommand* command);
	// 割り当て関数
	uint32_t Allocate();
	// CPUの特定のインデックスハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandleCPU(uint32_t index);
	// GPUの特定のインデックスハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorHandleGPU(uint32_t index);
	// DSV生成
	void CreateDSVTexture2d(uint32_t dsvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);
	// Viewの数が最大数を上回っているかどうか
	bool IsLowerViewMax();
	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	// ディスクリプタのサイズを取得
	uint32_t GetDescriptorSize() const;
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGIManager* dxgi);
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
private:
	// 最大dsv数
	static const uint32_t kMaxDSVCount_ = 1;
private:
	// ディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	// Discriptorのサイズ
	uint32_t descriptorSize_ = 0u;
	// 使用しているviewのインデックス
	uint32_t useIndex = 0;
private:
	// DXGIのインスタンスを受け取る箱
	DXGIManager* dxgi_ = nullptr;
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
};