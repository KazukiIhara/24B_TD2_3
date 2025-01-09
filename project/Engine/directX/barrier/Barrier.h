#pragma once

#include <d3d12.h>

class DirectXCommand;
class SwapChain;
class RenderTexture;

class Barrier {
public:
	Barrier() = default;
	~Barrier() = default;

	// 初期化
	void Initialize(DirectXCommand* command, SwapChain* swapChain, RenderTexture* renderTexture);

	// 描画前のバリア遷移
	void PostDrawBarrierTransition();
	// swapChainプレゼント前のバリア遷移
	void PreSwapChainPresentBarrierTransition();


private:
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// スワップチェーンのインスタンスをセット
	void SetSwapChain(SwapChain* swapChain);
	// レンダーテクスチャのインスタンスをセット
	void SetRenderTexture(RenderTexture* renderTexture);

private:
	// リソースバリア(スワップチェーン用)
	D3D12_RESOURCE_BARRIER swapChainBarrier_{};
	// リソースバリア(レンダーテクスチャ用)
	D3D12_RESOURCE_BARRIER renderTextureBarrier_{};
private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// スワップチェーンのインスタンスを受け取る箱
	SwapChain* swapChian_ = nullptr;
	// レンダーテクスチャのインスタンスを受け取る箱
	RenderTexture* renderTexture_ = nullptr;
};