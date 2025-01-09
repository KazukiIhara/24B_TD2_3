#include "ParticleSystem.h"

#include <cassert>

#include "directX/command/DirectXCommand.h"
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"

void ParticleSystem::Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager) {
	SetCommand(command);
	SetPipelineManager(pipelineManager);
}

void ParticleSystem::PreDraw() {
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	command_->GetList()->SetGraphicsRootSignature(pipelineManager_->GetRootSignature(kParticle));
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	command_->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleSystem::SetCommand(DirectXCommand* command) {
	assert(command);
	command_ = command;
}

void ParticleSystem::SetPipelineManager(GraphicsPipelineManager* pipelineManager) {
	assert(pipelineManager);
	pipelineManager_ = pipelineManager;
}
