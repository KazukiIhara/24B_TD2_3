#pragma once

class DirectXCommand;

class ViewPort {
public:
	ViewPort() = default;
	~ViewPort() = default;

	// 初期化
	void Initialize(DirectXCommand* command);

	// ビューポートを設定
	void SettingViewport();

private:
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);

private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;

};