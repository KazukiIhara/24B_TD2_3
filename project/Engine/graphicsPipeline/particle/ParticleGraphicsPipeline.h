#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// ヘッダファイル
#include "directX/includes/ComPtr.h"
#include "enum/GraphicsPipelineEnum.h"
#include "enum/BlendModeEnum.h"

class DXGIManager;

class ParticleGraphicsPipeline {
public:
	ParticleGraphicsPipeline() = default;
	~ParticleGraphicsPipeline() = default;

	// 初期化処理
	void Initialize(DXGIManager* dxgi);

	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();

	// 指定されたブレンドモードに対応するパイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState(BlendMode blendMode);

private:
	// ルートシグネチャを作成する
	void CreateRootSignature();

	// シェーダーをコンパイルする
	void CompileShaders();

	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject();

	// シェーダーコンパイラの初期化
	void InitializeDxCompiler();

	// シェーダーをコンパイルする
	ComPtr<ID3DBlob> CompileShader(
		// シェーダーファイルのパス
		const std::wstring& filePath,
		// コンパイルに使用するプロファイル
		const wchar_t* profile,
		// 初期化で生成したシェーダーコンパイラ関連の3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum);

	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting();

	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting();

	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting();

private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGIManager* dxgi);

private: // 静的メンバ変数
	// ブレンドモードの数
	static const uint32_t kBlendModeNum = 6;

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	// グラフィックスパイプラインステート (ブレンドモードごとに設定)
	ComPtr<ID3D12PipelineState> pipelineState_[kBlendModeNum];

	// 頂点シェーダーのバイナリデータ
	ComPtr<ID3DBlob> vertexShaderBlob_;

	// ピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlob_;

	// シェーダーコンパイルに使用するオブジェクト
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;

private:
	DXGIManager* dxgi_ = nullptr;
};