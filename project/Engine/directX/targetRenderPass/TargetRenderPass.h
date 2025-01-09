#pragma once

#include "math/types/Vector4.h"
#include "enum/RenderTargetEnum.h"

class DirectXCommand;
class SwapChain;
class RenderTexture;
class DepthStencil;

class TargetRenderPass {
public:
	TargetRenderPass() = default;
	~TargetRenderPass() = default;

	// 初期化
	void Initialize(DirectXCommand* command, SwapChain* swapChain, RenderTexture* renderTexture,DepthStencil* depthStencil);
	// レンダーターゲットのセット
	void SetRenderTarget(const RenderTargetType& type);
	// レンダーターゲットのクリア
	void ClearRenderTarget(const RenderTargetType& type);

	// クリアカラーのセット
	void SetClearColor(const Vector4& clearColor);
private:
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// スワップチェーンのインスタンスをセット
	void SetSwapChain(SwapChain* swapChain);
	// レンダーテクスチャのインスタンスをセット
	void SetRenderTexture(RenderTexture* renderTexture);
	// デプスステンシルをセット
	void SetDepthStencil(DepthStencil* depthStencil);
private:
	// 画面クリアカラー
	Vector4 clearColor_ = { 0.05f,0.05f,0.05f,1.0f };
private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// スワップチェーンのインスタンスを受け取る箱
	SwapChain* swapChain_ = nullptr;
	// レンダーテクスチャのインスタンスを受け取る箱
	RenderTexture* renderTexture_ = nullptr;
	// デプスステンシルのインスタンスを受け取る箱
	DepthStencil* depthStencil_ = nullptr;
};
