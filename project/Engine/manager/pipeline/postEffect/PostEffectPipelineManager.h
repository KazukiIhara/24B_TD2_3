#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "directX/includes/ComPtr.h"
#include "enum/PostEffectPipelineEnum.h"
#include "enum/BlendModeEnum.h"

#include "postEffectPipeline/none/NonePostEffectPipeline.h"

class DXGIManager;

class PostEffectPipelineManager {
public:
	PostEffectPipelineManager() = default;
	~PostEffectPipelineManager() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(PostEffectPipelineType pipelineType);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(PostEffectPipelineType pipelineType, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(PostEffectPipelineType pipelineType);

	// パイプラインをセット
	void SetPipelineState(PostEffectPipelineType pipelineType);

private:
	// パイプラインの種類の数 
	static const uint32_t kPostEffectPipelineStateNum = 1;
private: // メンバ変数
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kPostEffectPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineStates_[kPostEffectPipelineStateNum][BlendMode::blendModeNum];
private:
	// NonePostEffect
	std::unique_ptr<NonePostEffectPipeline> nonePostEffectPipeline_ = nullptr;
};