#pragma once

class DirectXCommand;

class ScissorRect {
public:
	ScissorRect() = default;
	~ScissorRect() = default;

	// 初期化
	void Initialize(DirectXCommand* command);

	// シザー矩形を設定
	void SettingScissorRect();

private:
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);

private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;

};