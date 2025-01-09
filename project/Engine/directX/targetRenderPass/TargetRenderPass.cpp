#include "TargetRenderPass.h"

#include <cassert>

#include "directX/command/DirectXCommand.h"
#include "directX/swapChain/SwapChain.h"
#include "directX/renderTexture/RenderTexture.h"
#include "directX/depthStencil/DepthStencil.h"

void TargetRenderPass::Initialize(DirectXCommand* command, SwapChain* swapChain, RenderTexture* renderTexture, DepthStencil* depthStencil) {
	SetCommand(command);
	SetSwapChain(swapChain);
	SetRenderTexture(renderTexture);
	SetDepthStencil(depthStencil);
}

void TargetRenderPass::SetRenderTarget(const RenderTargetType& type) {
	// RTVハンドルの受取先
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
	switch (type) {
	case RenderTargetType::kSwapChain:
		// SwapChainの現在のバックバッファのRTVハンドルを取得
		rtvHandle = swapChain_->GetCurrentBackBufferRTVHandle();
		break;
	case RenderTargetType::kRenderTexture:
		// RenderTextureのRTVハンドルを取得
		rtvHandle = renderTexture_->GetRTVHandle();
		break;
	}
	// デプスステンシルのDSVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvhandle = depthStencil_->GetDepthStencilResorceCPUHandle();
	// 描画先のRTVとDSVを指定
	command_->GetList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvhandle);
}

void TargetRenderPass::ClearRenderTarget(const RenderTargetType& type) {
	// RTVハンドルを受取先
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
	// クリアカラー
	float clearColor[4]{};
	switch (type) {
	case RenderTargetType::kSwapChain:
		// SwapChainの現在のバックバッファのRTVハンドルを取得
		rtvHandle = swapChain_->GetCurrentBackBufferRTVHandle();
		// クリアカラー変換
		clearColor[0] = clearColor_.x;
		clearColor[1] = clearColor_.y;
		clearColor[2] = clearColor_.z;
		clearColor[3] = clearColor_.w;
		break;
	case RenderTargetType::kRenderTexture:
		// RenderTextureのクリアカラーを取得
		Vector4 renderTextureClearColor = renderTexture_->GetClearColor();
		// クリアカラー変換
		clearColor[0] = renderTextureClearColor.x;
		clearColor[1] = renderTextureClearColor.y;
		clearColor[2] = renderTextureClearColor.z;
		clearColor[3] = renderTextureClearColor.w;
		// RenderTextureのRTVハンドルを取得
		rtvHandle = renderTexture_->GetRTVHandle();
		break;
	}
	// 指定したレンダーターゲットと色で画面全体をクリアする
	command_->GetList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void TargetRenderPass::SetClearColor(const Vector4& clearColor) {
	clearColor_ = clearColor;
}

void TargetRenderPass::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void TargetRenderPass::SetSwapChain(SwapChain* swapChain) {
	assert(swapChain);
	swapChain_ = swapChain;
}

void TargetRenderPass::SetRenderTexture(RenderTexture* renderTexture) {
	assert(renderTexture);
	renderTexture_ = renderTexture;
}

void TargetRenderPass::SetDepthStencil(DepthStencil* depthStencil) {
	assert(depthStencil);
	depthStencil_ = depthStencil;
}
