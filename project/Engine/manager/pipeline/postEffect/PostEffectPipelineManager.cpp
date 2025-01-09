#include "PostEffectPipelineManager.h"

// 標準C++ヘッダー
#include <cassert>
#include <format>  

// カスタムヘッダー
#include "debugTools/logger/Logger.h"  // ログ出力に使用
#include "directX/dxgi/DXGIManager.h"

void PostEffectPipelineManager::Initialize(DXGIManager* dxgi) {
	// NonePostEffectPipelineを生成、初期化
	nonePostEffectPipeline_ = std::make_unique<NonePostEffectPipeline>();
	nonePostEffectPipeline_->Initialize(dxgi);
	SetRootSignature(kPostEffectNone);
	SetPipelineState(kPostEffectNone);
}

ID3D12RootSignature* PostEffectPipelineManager::GetRootSignature(PostEffectPipelineType pipelineType) {
	return rootSignatures_[pipelineType].Get();
}

ID3D12PipelineState* PostEffectPipelineManager::GetPipelineState(PostEffectPipelineType pipelineType, BlendMode blendMode) {
	return pipelineStates_[pipelineType][blendMode].Get();
}

void PostEffectPipelineManager::SetRootSignature(PostEffectPipelineType pipelineType) {
	// タイプに応じたルートシグネイチャを設定
	switch (pipelineType) {
	case kPostEffectNone:
		rootSignatures_[pipelineType] = nonePostEffectPipeline_->GetRootSignature();
		break;
	}
}

void PostEffectPipelineManager::SetPipelineState(PostEffectPipelineType pipelineType) {
	// タイプに応じたパイプラインを設定
	switch (pipelineType) {
	case kPostEffectNone:
		for (int mode = kBlendModeNone; mode < blendModeNum; ++mode) {
			pipelineStates_[pipelineType][mode] = nonePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	}
}
