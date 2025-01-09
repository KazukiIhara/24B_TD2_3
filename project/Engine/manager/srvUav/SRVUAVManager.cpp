// This
#include "SRVUAVManager.h"

// MyHedder
#include <cassert>

#include "directX/dxgi/DXGIManager.h"
#include "directX/command/DirectXCommand.h"


void SRVUAVManager::Initialize(DXGIManager* dxgi, DirectXCommand* command) {
	SetDXGI(dxgi);
	SetCommand(command);
	// デスクリプタヒープの作成
	descriptorHeap_ = dxgi_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxViewCount_, true);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize_ = dxgi_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t SRVUAVManager::Allocate() {
	int index = useIndex;
	useIndex++;
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SRVUAVManager::GetDescriptorHandleCPU(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRVUAVManager::GetDescriptorHandleGPU(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

void SRVUAVManager::CreateSrvTexture2d(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {
	// metaDataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;
	// Srvの作成
	dxgi_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetDescriptorHandleCPU(srvIndex));
}

void SRVUAVManager::CreateSrvStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;
	dxgi_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetDescriptorHandleCPU(srvIndex));
}

void SRVUAVManager::CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = numElements;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = structureByteStride;

	dxgi_->GetDevice()->CreateUnorderedAccessView(
		pResource, nullptr, &uavDesc, GetDescriptorHandleCPU(viewIndex));
}

bool SRVUAVManager::IsLowerViewMax() {
	return useIndex < kMaxViewCount_;
}

void SRVUAVManager::PreCommand() {
	// DescriptorHeapを設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { descriptorHeap_ };
	command_->GetList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
}

void SRVUAVManager::SetComputeRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) {
	command_->GetList()->SetComputeRootDescriptorTable(rootParameterIndex, GetDescriptorHandleGPU(srvIndex));
}

void SRVUAVManager::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) {
	command_->GetList()->SetGraphicsRootDescriptorTable(rootParameterIndex, GetDescriptorHandleGPU(srvIndex));
}

ID3D12DescriptorHeap* SRVUAVManager::GetDescriptorHeap() const {
	return descriptorHeap_.Get();
}

uint32_t SRVUAVManager::GetDescriptorSize() const {
	return descriptorSize_;
}

void SRVUAVManager::SetDXGI(DXGIManager* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SRVUAVManager::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}
