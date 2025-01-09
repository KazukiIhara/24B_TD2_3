#pragma once

// WindowsAPI
#include <Windows.h>

// C++
#include <cstdint>

// imgui
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

// 前方宣言
class WindowManager;
class DXGIManager;
class DirectXCommand;
class SRVUAVManager;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
);

// imguiマネージャ
class ImGuiManager {
public: // 公開メンバ関数
	ImGuiManager() = default;
	~ImGuiManager() = default;

	// 初期化
	void Initialize(WindowManager* windowManager, DXGIManager* dxgi, DirectXCommand* command, SRVUAVManager* srvManager);

	// フォント設定
	void FontSetting();

	// 更新処理の先頭に呼び出す処理
	void BeginFrame();

	// 更新処理の最後に呼び出す処理
	void EndFrame();

	// 描画
	void Draw();


	// ImGuiの終了処理
	void Finalize();

	// ImGuiの配色をダークテーマにする
	void SetImGuiDarkTheme();

private:
	void SetWindowManager(WindowManager* windowManager);
	void SetDXGI(DXGIManager* dxgi);
	void SetCommand(DirectXCommand* command);
	void SetSrvManager(SRVUAVManager* srvManager);

private:
	// プレビュー描画のSrvIndex
	uint32_t previewSrvIndex_;

private: // インスタンスを受け取る変数
	// WinAPI
	WindowManager* windowManager_ = nullptr;
	// dxgi
	DXGIManager* dxgi_ = nullptr;
	// command
	DirectXCommand* command_ = nullptr;
	// SrvManager
	SRVUAVManager* srvUavManager_ = nullptr;
};

