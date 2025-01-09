#include "Object3dSystem.h"

#include <cassert>

#include "directX/command/DirectXCommand.h"
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"

void Object3DSystem::Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager) {
	SetCommand(command);
	SetPipelineManager(pipelineManager);
}

void Object3DSystem::PreDraw() {
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	command_->GetList()->SetGraphicsRootSignature(pipelineManager_->GetRootSignature(kObject3d));
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	command_->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3DSystem::PreDrawSkinning() {
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	command_->GetList()->SetGraphicsRootSignature(pipelineManager_->GetRootSignature(kObject3dSkinning));
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	command_->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3DSystem::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void Object3DSystem::SetPipelineManager(GraphicsPipelineManager* pipelineManager) {
	assert(pipelineManager);
	pipelineManager_ = pipelineManager;
}
