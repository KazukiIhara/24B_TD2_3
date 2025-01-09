#pragma once

// 前方宣言
class DirectXCommand;
class GraphicsPipelineManager;

class LineSystem {
public:
	LineSystem() = default;
	~LineSystem() = default;

	// 初期化処理
	void Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager);

	// 描画前処理
	void PreDraw();
private:
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// PipelineManager
	void SetPipelineManager(GraphicsPipelineManager* pipelineManager);

private: // インスタンスを受け取るポインタ
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// PipelineManager
	GraphicsPipelineManager* pipelineManager_ = nullptr;

};