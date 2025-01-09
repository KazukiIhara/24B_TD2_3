#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "directX/includes/ComPtr.h"
#include "enum/GraphicsPipelineEnum.h"
#include "enum/BlendModeEnum.h"

#include "graphicsPipeline/object2d/Object2dGraphicsPipeline.h"
#include "graphicsPipeline/ocject3d/Object3dGraphicsPipeline.h"
#include "graphicsPipeline/object3dSkinning/Object3dSkinningGraphicsPipeline.h"
#include "graphicsPipeline/particle/ParticleGraphicsPipeline.h"
#include "graphicsPipeline/line/LineGraphicsPipeline.h"

class DXGIManager;

class GraphicsPipelineManager {
public: // 公開メンバ変数
	GraphicsPipelineManager() = default;
	~GraphicsPipelineManager() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(GraphicsPipelineStateType pipelineState);

private: // 静的メンバ変数
	// パイプラインの種類の数 
	static const uint32_t kGraphicsPipelineStateNum = 5;
private: // メンバ変数
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kGraphicsPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> graphicsPipelineStates_[kGraphicsPipelineStateNum][BlendMode::blendModeNum];
private:
	// 2DGraphicsPipeline
	std::unique_ptr<Object2DGraphicsPipeline> object2dGraphicsPipeline_ = nullptr;
	// 3DGraphicsPipeline
	std::unique_ptr<Object3DGraphicsPipeline> object3dGraphicsPipeline_ = nullptr;
	// Skinning3DGraphicsPipeline
	std::unique_ptr<Object3DSkinningGraphicsPipeline> object3dSkinningGraphicsPipeline_ = nullptr;
	// ParticlePipeline
	std::unique_ptr<ParticleGraphicsPipeline> particleGraphicsPipeline_ = nullptr;
	// LinePipeline
	std::unique_ptr<LineGraphicsPipeline> lineGraphicsPipeline_ = nullptr;
};
