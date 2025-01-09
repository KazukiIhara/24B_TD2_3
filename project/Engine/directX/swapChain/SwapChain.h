#pragma once

// C++
#include <cmath>
#include <cassert>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// 自作ファイル
#include "directX/includes/ComPtr.h"

class DXGIManager;
class DirectXCommand;
class WindowManager;
class RTVManager;

class SwapChain {
public:
	SwapChain() = default;
	~SwapChain() = default;
	// 初期化
	void Initialize(WindowManager* windowmanager, DXGIManager* dxgi, DirectXCommand* command, RTVManager* rtvManager);
	// GPUとOSに画面の交換を行うよう通知する
	void Present();
	// 現在のバックバッファのリソースを取得
	ID3D12Resource* GetCurrentBackBufferResource();
	// 描画先のRTVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTVHandle();
private:
	// スワップチェーン作成
	void CreateSwapChain();
	// スワップチェーンのRTVリソースを作成
	void CreateResources();
	// RTVを作成
	void CreateRTV();
private:
	// WindowManagerをセット
	void SetWindowManager(WindowManager* windowManager);
	// DXGIをセット
	void SetDXGI(DXGIManager* dxgi);
	// Commandをセット
	void SetCommand(DirectXCommand* command);
	// RTVManagerのセット
	void SetRTVManager(RTVManager* rtvManager);
private:
	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	// スワップチェーンリソース
	ComPtr<ID3D12Resource> swapChainResources_[2] = { nullptr };
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// RTVリソースのインデックス
	uint32_t rtvIndex_[2]{};
	// バックバッファインデックス
	UINT backBufferIndex_ = 0;
	// エラー判別君
	HRESULT hr_ = S_FALSE;
private:
	// WindowManagerのインスタンスを受け取る箱
	WindowManager* windowManager_ = nullptr;
	// dxgiのインスタンスを受け取る箱
	DXGIManager* dxgi_ = nullptr;
	// Commandのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// RTVmanagerのインスタンスを受け取る箱
	RTVManager* rtvManager_ = nullptr;
};