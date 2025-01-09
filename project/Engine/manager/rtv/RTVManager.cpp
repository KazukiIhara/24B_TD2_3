#include "RTVManager.h"

#include <cassert>

#include "directX/dxgi/DXGIManager.h"
#include "directX/command/DirectXCommand.h"

void RTVManager::Initialize(DXGIManager* dxgi) {
	// DXGIのセット
	SetDXGI(dxgi);
	// デスクリプタヒープの作成
	descriptorHeap_ = dxgi_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxRTVCount, false);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize_ = dxgi_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

uint32_t RTVManager::Allocate() {
	int index = useIndex;
	useIndex++;
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetDescriptorHandleCPU(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE RTVManager::GetDescriptorHandleGPU(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

void RTVManager::CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource) {
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;		// 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む
	dxgi_->GetDevice()->CreateRenderTargetView(pResource, &rtvDesc_, GetDescriptorHandleCPU(rtvIndex));
}

void RTVManager::SetDXGI(DXGIManager* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}