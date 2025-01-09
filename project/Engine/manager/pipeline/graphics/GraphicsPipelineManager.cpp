// DirectX 12でルートシグネチャとグラフィックスパイプラインの管理を行う

// GraphicsPipelineManagerクラスのヘッダーファイルをインクルード
#include "GraphicsPipelineManager.h"

// 標準C++ヘッダー
#include <cassert>
#include <format>  

// カスタムヘッダー
#include "debugTools/logger/Logger.h"  // ログ出力に使用
#include "directX/dxgi/DXGIManager.h"

// GraphicsPipelineManagerの初期化処理
// DirectXManagerを使用して2D用のグラフィックスパイプラインを設定
void GraphicsPipelineManager::Initialize(DXGIManager* dxgi) {
	// 2Dオブジェクトのグラフィックスパイプラインを生成、初期化
	object2dGraphicsPipeline_ = std::make_unique<Object2DGraphicsPipeline>();
	object2dGraphicsPipeline_->Initialize(dxgi);
	SetRootSignature(kObject2d);
	SetPipelineState(kObject2d);

	// 3Dオブジェクトのグラフィックスパイプラインを生成、初期化
	object3dGraphicsPipeline_ = std::make_unique<Object3DGraphicsPipeline>();
	object3dGraphicsPipeline_->Initialize(dxgi);
	SetRootSignature(kObject3d);
	SetPipelineState(kObject3d);

	// Skinning3Dオブジェクトのグラフィックスパイプラインを生成、初期化
	object3dSkinningGraphicsPipeline_ = std::make_unique<Object3DSkinningGraphicsPipeline>();
	object3dSkinningGraphicsPipeline_->Initialize(dxgi);
	SetRootSignature(kObject3dSkinning);
	SetPipelineState(kObject3dSkinning);

	// Particleグラフィックスパイプラインを生成、初期化
	particleGraphicsPipeline_ = std::make_unique<ParticleGraphicsPipeline>();
	particleGraphicsPipeline_->Initialize(dxgi);
	SetRootSignature(kParticle);
	SetPipelineState(kParticle);

	// Lineグラフィックスパイプラインを生成、初期化
	lineGraphicsPipeline_ = std::make_unique<LineGraphicsPipeline>();
	lineGraphicsPipeline_->Initialize(dxgi);
	SetRootSignature(kLine);
	SetPipelineState(kLine);

}

// 指定されたパイプラインステートに対応するルートシグネチャを取得する
ID3D12RootSignature* GraphicsPipelineManager::GetRootSignature(GraphicsPipelineStateType pipelineState) {
	// 該当するパイプラインステートのルートシグネチャを返す
	return rootSignatures_[pipelineState].Get();
}

// 指定されたパイプラインステートとブレンドモードに対応するパイプラインステートオブジェクトを取得する
ID3D12PipelineState* GraphicsPipelineManager::GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	// 該当するパイプラインステートとブレンドモードに対応するパイプラインステートオブジェクトを返す
	return graphicsPipelineStates_[pipelineState][blendMode].Get();
}

// 指定されたパイプラインステートに対応するルートシグネチャを設定する
void GraphicsPipelineManager::SetRootSignature(GraphicsPipelineStateType pipelineState) {
	// パイプラインステートごとに対応するルートシグネチャを設定
	switch (pipelineState) {
	case kObject2d:
		// 2Dオブジェクト描画用のルートシグネチャを設定
		rootSignatures_[pipelineState] = object2dGraphicsPipeline_->GetRootSignature();
		break;
	case kObject3d:
		// 3Dオブジェクト描画用のルートシグネチャを設定
		rootSignatures_[pipelineState] = object3dGraphicsPipeline_->GetRootSignature();
		break;
	case kObject3dSkinning:
		// スキニング3Dオブジェクト描画用のルートシグネイチャを設定
		rootSignatures_[pipelineState] = object3dSkinningGraphicsPipeline_->GetRootSignature();
		break;
	case kParticle:
		// パーティクル描画用のルートシグネチャを設定
		rootSignatures_[pipelineState] = particleGraphicsPipeline_->GetRootSignature();
		break;

	case kLine:
		// ライン描画用のルートシグネイチャを設定
		rootSignatures_[pipelineState] = lineGraphicsPipeline_->GetRootSignature();
		break;


		// 他のパイプラインステートが追加された場合はここに追加
	}
}

// 指定されたパイプラインステートに対応するグラフィックスパイプラインステートを設定する
void GraphicsPipelineManager::SetPipelineState(GraphicsPipelineStateType pipelineState) {
	// パイプラインステートごとに対応するグラフィックスパイプラインを設定
	switch (pipelineState) {
	case kObject2d:
		for (int mode = kBlendModeNone; mode < blendModeNum; ++mode) {
			graphicsPipelineStates_[pipelineState][mode] = object2dGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case kObject3d:
		for (int mode = kBlendModeNone; mode < blendModeNum; ++mode) {
			graphicsPipelineStates_[pipelineState][mode] = object3dGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case kObject3dSkinning:
		for (int mode = kBlendModeNone; mode < blendModeNum; ++mode) {
			graphicsPipelineStates_[pipelineState][mode] = object3dSkinningGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case kParticle:
		for (int mode = kBlendModeNone; mode < blendModeNum; ++mode) {
			graphicsPipelineStates_[pipelineState][mode] = particleGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case kLine:
		for (int mode = kBlendModeNone; mode < blendModeNum; ++mode) {
			graphicsPipelineStates_[pipelineState][mode] = lineGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;

		// 他のパイプラインステートが追加された場合はここに追加
	}
}
