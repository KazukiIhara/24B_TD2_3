#include "ComputePipelineManager.h"
#include "directX/dxgi/DXGIManager.h"

void ComputePipelineManager::Initialize(DXGIManager* dxgi) {
	// Skinning用のパイプラインを作成、初期化
	skinningComputePipeline_ = std::make_unique<SkinningComputePipeline>();
	skinningComputePipeline_->Initialize(dxgi);
	SetRootSignature(kSkinning);
	SetPipelineState(kSkinning);
}

ID3D12RootSignature* ComputePipelineManager::GetRootSignature(ComputePipelineStateType pipelineState) {
	// 該当するパイプラインステートのルートシグネチャを返す
	return rootSignatures_[pipelineState].Get();
}

ID3D12PipelineState* ComputePipelineManager::GetPipelineState(ComputePipelineStateType pipelineState) {
	// 該当するパイプラインステートとブレンドモードに対応するパイプラインステートオブジェクトを返す
	return graphicsPipelineStates_[pipelineState].Get();
}

void ComputePipelineManager::SetRootSignature(ComputePipelineStateType pipelineState) {
	// パイプラインごとに対応するルートイグネイチャを設定
	switch (pipelineState) {
	case kSkinning:
		// Skinning
		rootSignatures_[pipelineState] = skinningComputePipeline_->GetRootSignature();
		break;
	}
}

void ComputePipelineManager::SetPipelineState(ComputePipelineStateType pipelineState) {
	// パイプラインごとに対応するルートイグネイチャを設定
	switch (pipelineState) {
	case kSkinning:
		// Skinning
		graphicsPipelineStates_[pipelineState] = skinningComputePipeline_->GetPipelineState();
		break;
	}
}
