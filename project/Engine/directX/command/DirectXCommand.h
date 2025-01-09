#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

// 自作ファイル
#include "directX/includes/ComPtr.h"

class DXGIManager;

class DirectXCommand {
public:
	DirectXCommand() = default;
	~DirectXCommand() = default;

	// 初期化
	void Initialize(DXGIManager* dxgi);

	// コマンドの実行
	void KickCommand();

	// コマンドのリセット
	void ResetCommand();

	// キューの取得
	ID3D12CommandQueue* GetQueue();
	// アロケータの取得
	ID3D12CommandAllocator* GetAllocator();
	// リストの取得
	ID3D12GraphicsCommandList* GetList();

private:
	// DXGIのセット
	void SetDXGIManager(DXGIManager* dxgi);
private:
	// SUCCEEDEDでエラー判別君
	HRESULT hr_ = S_FALSE;

	// DXGI
	DXGIManager* dxgi_ = nullptr;

	// コマンドキュー
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	// コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	// コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

};