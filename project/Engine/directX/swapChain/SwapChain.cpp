#include "SwapChain.h"

#include <cassert>

#include "manager/window/WindowManager.h"
#include "directX/dxgi/DXGIManager.h"
#include "directX/command/DirectXCommand.h"
#include "manager/rtv/RTVManager.h"

void SwapChain::Initialize(WindowManager* windowmanager, DXGIManager* dxgi, DirectXCommand* command, RTVManager* rtvManager) {
	// WindowManagerのインスタンスをセット
	SetWindowManager(windowmanager);
	// DXGIのインスタンスをセット
	SetDXGI(dxgi);
	// Commandのセット
	SetCommand(command);
	// RTVManagerのインスタンスをセット
	SetRTVManager(rtvManager);

	// スワップチェーン作成
	CreateSwapChain();
	// リソースを作成
	CreateResources();
	// RTVを作成
	CreateRTV();
}

void SwapChain::Present() {
	swapChain_->Present(1, 0);
}

ID3D12Resource* SwapChain::GetCurrentBackBufferResource() {
	backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();
	return swapChainResources_[backBufferIndex_].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetCurrentBackBufferRTVHandle() {
	backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();
	return rtvManager_->GetDescriptorHandleCPU(rtvIndex_[backBufferIndex_]);
}

void SwapChain::CreateSwapChain() {
	// スワップチェーンの設定
	swapChainDesc_.Width = WindowManager::kClientWidth;			// 画面の幅、ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = WindowManager::kClientHeight;		// 画面の高さ、ウィンドウのクライアント領域を同じものしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// 色形式
	swapChainDesc_.SampleDesc.Count = 1;						// マルチサンプルしない
	swapChainDesc_.BufferCount = 2;								// ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタにうつしたら、中身を破棄
	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr_ = dxgi_->GetFactory()->CreateSwapChainForHwnd(command_->GetQueue(), windowManager_->GetHwnd(), &swapChainDesc_,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr_));
}

void SwapChain::CreateResources() {
	// スワップチェーンからリソースを引っ張ってくる
	hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	// うまく取得出来なければ起動できない
	assert(SUCCEEDED(hr_));
	hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr_));
}

void SwapChain::CreateRTV() {
	// 1つめ
	rtvIndex_[0] = rtvManager_->Allocate();
	rtvManager_->CreateRTVTexture2d(rtvIndex_[0], swapChainResources_[0].Get());
	// 2つめ
	rtvIndex_[1] = rtvManager_->Allocate();
	rtvManager_->CreateRTVTexture2d(rtvIndex_[1], swapChainResources_[1].Get());
}

void SwapChain::SetDXGI(DXGIManager* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SwapChain::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void SwapChain::SetRTVManager(RTVManager* rtvManager) {
	assert(rtvManager);
	rtvManager_ = rtvManager;
}

void SwapChain::SetWindowManager(WindowManager* windowManager) {
	assert(windowManager);
	windowManager_ = windowManager;
}
