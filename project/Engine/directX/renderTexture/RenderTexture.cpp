#include "RenderTexture.h"

#include <cassert>

#include "manager/window/WindowManager.h"
#include "directX/dxgi/DXGIManager.h"
#include "directX/command/DirectXCommand.h"
#include "manager/rtv/RTVManager.h"
#include "manager/srvUav/SRVUAVManager.h"
#include "manager/pipeline/postEffect/PostEffectPipelineManager.h"

void RenderTexture::Initialize(DXGIManager* dxgi, DirectXCommand* command, RTVManager* rtvManager, SRVUAVManager* srvUavManager, PostEffectPipelineManager* postEffectPipelineManager) {
	// インスタンスをセット
	SetDXGI(dxgi);
	SetCommand(command);
	SetRTVManager(rtvManager);
	SetSRVUAVManager(srvUavManager);
	SetPostEffectPipelineManager(postEffectPipelineManager);

	// リソースを作成
	CreateResource();
	// RTVを作成
	CreateRTV();
	// SRVを作成
	CreateSRV();
}

void RenderTexture::Draw() {
	// ルートシグネイチャを設定
	command_->GetList()->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(postEffectType_));
	// PSOを設定
	command_->GetList()->SetPipelineState(postEffectPipelineManager_->GetPipelineState(postEffectType_, blendMode_));
	// ディスクリプタテーブルを設定
	srvUavManager_->SetGraphicsRootDescriptorTable(0, srvIndex_);

	command_->GetList()->DrawInstanced(3, 1, 0, 0);
}

Vector4 RenderTexture::GetClearColor()const {
	return kClearColor_;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetRTVHandle() {
	return rtvManager_->GetDescriptorHandleCPU(rtvIndex_);
}

ID3D12Resource* RenderTexture::GetResource() {
	return resource_.Get();
}

void RenderTexture::CreateResource() {
	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(WindowManager::kClientWidth);				// Textureの幅
	resourceDesc.Height = UINT(WindowManager::kClientHeight);			// Textureの高さ
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;									// サンプリングカウント。1固定
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;		// renderTargetとして利用可能にする
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// クリアカラーの設定
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = kClearColor_.x;
	clearValue.Color[1] = kClearColor_.y;
	clearValue.Color[2] = kClearColor_.z;
	clearValue.Color[3] = kClearColor_.w;

	// リソースの作成
	resource_ = nullptr;
	hr_ = dxgi_->GetDevice()->CreateCommittedResource(
		&heapProperties,								// Heapの設定
		D3D12_HEAP_FLAG_NONE,							// Heapの特殊な設定。特になし。
		&resourceDesc,									// リソースの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET,				// これから描画することを前提としたTextureなので、RenderTargetとして使うことから始める
		&clearValue,									// Clear最適値、ClearRenderをこの色でクリアするようにする、最適化されているので高速である。
		IID_PPV_ARGS(&resource_)
	);
	assert(SUCCEEDED(hr_));
}

void RenderTexture::CreateRTV() {
	// インデックスを割り当て
	rtvIndex_ = rtvManager_->Allocate();
	// RTVを作成
	rtvManager_->CreateRTVTexture2d(rtvIndex_, resource_.Get());
}

void RenderTexture::CreateSRV() {
	// インデックス割りて
	srvIndex_ = srvUavManager_->Allocate();
	// SRVを作成
	srvUavManager_->CreateSrvTexture2d(srvIndex_, resource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
}

void RenderTexture::SetDXGI(DXGIManager* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void RenderTexture::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void RenderTexture::SetRTVManager(RTVManager* rtvManager) {
	assert(rtvManager);
	rtvManager_ = rtvManager;
}

void RenderTexture::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void RenderTexture::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager) {
	assert(postEffectPipelineManager);
	postEffectPipelineManager_ = postEffectPipelineManager;
}
