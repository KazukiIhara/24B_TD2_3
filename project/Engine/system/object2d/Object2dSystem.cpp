// This
#include "Object2dSystem.h"

#include <cassert>

// MyHedder
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"
#include "directX/command/DirectXCommand.h"

void Object2DSystem::Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager) {
	SetCommand(command);
	SetPipelineManager(pipelineManager);
}

void Object2DSystem::PreDraw() {
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	command_->GetList()->SetGraphicsRootSignature(pipelineManager_->GetRootSignature(kObject2d));
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	command_->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object2DSystem::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void Object2DSystem::SetPipelineManager(GraphicsPipelineManager* pipelineManager) {
	assert(pipelineManager);
	pipelineManager_ = pipelineManager;
}
