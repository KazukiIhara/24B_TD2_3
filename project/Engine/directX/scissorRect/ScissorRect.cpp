#include "ScissorRect.h"

#include <cassert>

#include "directX/command/DirectXCommand.h"
#include "manager/window/WindowManager.h"

void ScissorRect::Initialize(DirectXCommand* command) {
	SetCommand(command);
}

void ScissorRect::SettingScissorRect() {
	// シザー矩形の設定
	D3D12_RECT scissorRect{};
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WindowManager::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WindowManager::kClientHeight;
	command_->GetList()->RSSetScissorRects(1, &scissorRect);
}

void ScissorRect::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}
