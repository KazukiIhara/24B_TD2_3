#pragma once

// C++
#include <cmath>
#include <cassert>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// 自作ファイル
#include "directX/includes/ComPtr.h"
#include "enum/BlendModeEnum.h"
#include "enum/PostEffectPipelineEnum.h"
#include "math/types/Vector4.h"

class DXGIManager;
class DirectXCommand;
class RTVManager;
class SRVUAVManager;
class PostEffectPipelineManager;

class RenderTexture {
public:
	RenderTexture() = default;
	~RenderTexture() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi, DirectXCommand* command, RTVManager* rtvManager, SRVUAVManager* srvUavManager, PostEffectPipelineManager* postEffectPipelineManager);
	// 描画
	void Draw();

	// クリアカラーを取得
	Vector4 GetClearColor()const;
	// RTVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();
	// リソースを取得
	ID3D12Resource* GetResource();
private:
	// レンダーテクスチャのリソースを作成
	void CreateResource();
	// RTVを作成
	void CreateRTV();
	// SRVを作成
	void CreateSRV();
private:
	// DXGIをセット
	void SetDXGI(DXGIManager* dxgi);
	// Commandをセット
	void SetCommand(DirectXCommand* command);
	// RTVManagerのセット
	void SetRTVManager(RTVManager* rtvManager);
	// SRVUAVManagerのセット
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	// PostEffectPipelineManagerのセット
	void SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager);
private:
	// レンダーテクスチャ用のリソース
	ComPtr<ID3D12Resource> resource_ = nullptr;
	// RTVリソースのインデックス
	uint32_t rtvIndex_ = 0;
	// SRVリソースのインデックス
	uint32_t srvIndex_ = 0;
	// クリアカラー
	const Vector4 kClearColor_ = { 0.0f,0.0f,1.0f,1.0f };
	// ポストエフェクトのタイプ
	PostEffectPipelineType postEffectType_ = kPostEffectNone;
	// ブレンドモード
	BlendMode blendMode_ = kBlendModeNone;
	// エラー判別君
	HRESULT hr_ = S_FALSE;
private:
	// dxgiのインスタンスを受け取る箱
	DXGIManager* dxgi_ = nullptr;
	// Commandのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// RTVmanagerのインスタンスを受け取る箱
	RTVManager* rtvManager_ = nullptr;
	// SRVUAVManagerのインスタンスを受け取る箱
	SRVUAVManager* srvUavManager_ = nullptr;
	// PostEffectPipelineManagerのインスタンスを受け取る箱
	PostEffectPipelineManager* postEffectPipelineManager_ = nullptr;
};