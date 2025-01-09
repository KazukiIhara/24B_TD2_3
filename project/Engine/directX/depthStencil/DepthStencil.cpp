#include "DepthStencil.h"

#include <cassert>

#include "manager/window/WindowManager.h"
#include "directX/dxgi/DXGIManager.h"
#include "directX/command/DirectXCommand.h"
#include "manager/dsv/DSVManager.h"

void DepthStencil::Initialize(DXGIManager* dxgi, DirectXCommand* command, DSVManager* dsvManager) {
	// DXGIのセット
	SetDXGI(dxgi);
	// コマンドをセット
	SetCommand(command);
	// DSVマネージャをセット
	SetDSVManager(dsvManager);
	// DepthStencilViewを作成
	CreateDepthStencilView();
}

void DepthStencil::ClearDepthView() {
	// 指定した深度で画面全体をクリア
	command_->GetList()->ClearDepthStencilView(
		dsvManager_->GetDescriptorHandleCPU(dsvIndex_),
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE DepthStencil::GetDepthStencilResorceCPUHandle() {
	return dsvManager_->GetDescriptorHandleCPU(dsvIndex_);
}

void DepthStencil::CreateDepthStencilView() {
	// DepthStencilTextureをウィンドウのサイズで作成
	depthStencilResource_ = dxgi_->CreateDepthStencilTextureResource(
		WindowManager::kClientWidth,
		WindowManager::kClientHeight
	);
	// インデックス割り当て
	dsvIndex_ = dsvManager_->Allocate();
	// dsv作成
	dsvManager_->CreateDSVTexture2d(
		dsvIndex_,
		depthStencilResource_.Get(),
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);
}

void DepthStencil::SetDXGI(DXGIManager* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void DepthStencil::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void DepthStencil::SetDSVManager(DSVManager* dsvManager) {
	assert(dsvManager);
	dsvManager_ = dsvManager;
}
