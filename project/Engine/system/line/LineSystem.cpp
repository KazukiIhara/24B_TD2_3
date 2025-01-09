#include "LineSystem.h"

#include <cassert>

#include "directX/command/DirectXCommand.h"
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"

void LineSystem::Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager) {
	SetCommand(command);
	SetPipelineManager(pipelineManager);
}

void LineSystem::PreDraw() {
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	command_->GetList()->SetGraphicsRootSignature(pipelineManager_->GetRootSignature(kLine));
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	command_->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineSystem::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void LineSystem::SetPipelineManager(GraphicsPipelineManager* pipelineManager) {
	assert(pipelineManager);
	pipelineManager_ = pipelineManager;
}
