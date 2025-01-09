#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "directX/includes/ComPtr.h"
#include "enum/ComputePipelineEnum.h"

#include "computePipeline/skinning/SkinningComputePipeline.h"

class DXGIManager;

class ComputePipelineManager {
public:
	ComputePipelineManager() = default;
	~ComputePipelineManager() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(ComputePipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(ComputePipelineStateType pipelineState);

	void SetRootSignature(ComputePipelineStateType pipelineState);

	void SetPipelineState(ComputePipelineStateType pipelineState);

private:
	// パイプラインの種類の数 
	static const uint32_t kComputePipelineStateNum = 1;

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kComputePipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> graphicsPipelineStates_[kComputePipelineStateNum];
private:
	// Skinning用のComputePipeline
	std::unique_ptr<SkinningComputePipeline> skinningComputePipeline_ = nullptr;

};