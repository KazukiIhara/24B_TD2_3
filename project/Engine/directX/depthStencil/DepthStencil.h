#pragma once

#include <cstdint>

#include <d3d12.h>

#include "directX/includes/ComPtr.h"

class DXGIManager;
class DirectXCommand;
class DSVManager;

class DepthStencil {
public:
	DepthStencil() = default;
	~DepthStencil() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi, DirectXCommand* command, DSVManager* dsvManager);
	// デプスをクリア
	void ClearDepthView();
	// デプスステンシルリソースのCPUハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilResorceCPUHandle();
private:
	// デプスステンシルリソースの作成
	void CreateDepthStencilView();
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGIManager* dxgi);
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// DSVマネージャのインスタンスをセット
	void SetDSVManager(DSVManager* dsvManager);
private:
	// デプスステンシルリソース
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	// デプスステンシルリソースのインデックス
	uint32_t dsvIndex_ = 0;
private:
	// DXGIのインスタンスを受け取る箱
	DXGIManager* dxgi_ = nullptr;
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// DSVマネージャのインスタンスを受け取る箱
	DSVManager* dsvManager_ = nullptr;
};