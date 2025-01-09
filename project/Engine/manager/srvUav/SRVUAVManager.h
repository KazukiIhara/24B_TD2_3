#pragma once

// C++
#include <cstdint>

// DirectX
#include <d3d12.h>

#include "directX/includes/ComPtr.h"

// 前方宣言
class DXGIManager;
class DirectXCommand;

class SRVUAVManager {
public: // 公開メンバ関数
	SRVUAVManager() = default;
	~SRVUAVManager() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi, DirectXCommand* command);
	// 割り当て関数
	uint32_t Allocate();
	// CPUの特定のインデックスハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandleCPU(uint32_t index);
	// GPUの特定のインデックスハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorHandleGPU(uint32_t index);
	// SRV生成(テクスチャ用)
	void CreateSrvTexture2d(uint32_t viewIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT MipLevels);
	// SRV生成(StructuredBuffer用)
	void CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
	// UAV作成
	void CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);

	// Viewの数が最大数を上回っているかどうか
	bool IsLowerViewMax();
	// コマンド前処理
	void PreCommand();
	// ルートパラメータインデックスと、srv番号に対応したDescriptorTableを取得(compute用)
	void SetComputeRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);
	// ルートパラメータインデックスと、srv番号に対応したDescriptorTableを取得(graphics用)
	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);

	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* GetDescriptorHeap() const;

	// ディスクリプタのサイズを取得
	uint32_t GetDescriptorSize() const;

private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGIManager* dxgi);
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
public: // 公開メンバ変数
	// 最大SRV数
	static const uint32_t kMaxViewCount_ = 512;
private: // 非公開メンバ変数
	// DescriptorHeap
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	// Discriptorのサイズ
	uint32_t descriptorSize_ = 0u;
	// 使用しているviewのインデックス
	uint32_t useIndex = 0;
private: // インスタンスコピーポインタ
	// DXGIのインスタンスを受け取る箱
	DXGIManager* dxgi_ = nullptr;
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
};