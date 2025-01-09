#include "ViewPort.h"

#include <cassert>

#include "directX/command/DirectXCommand.h"
#include "manager/window/WindowManager.h"

void ViewPort::Initialize(DirectXCommand* command) {
	SetCommand(command);
}

void ViewPort::SettingViewport() {
	// ビューポートの設定
	D3D12_VIEWPORT viewport{};
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = WindowManager::kClientWidth;
	viewport.Height = WindowManager::kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	command_->GetList()->RSSetViewports(1, &viewport);
}


void ViewPort::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}
