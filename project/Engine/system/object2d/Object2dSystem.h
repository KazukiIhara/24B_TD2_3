#pragma once

// 前方宣言
class DirectXCommand;
class GraphicsPipelineManager;

class Object2DSystem {
public: // 公開メンバ変数
	Object2DSystem() = default;
	~Object2DSystem() = default;

	// 初期化
	void Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager);

	// 描画前処理
	void PreDraw();
private:
	// commandのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// PipelineManager
	void SetPipelineManager(GraphicsPipelineManager* pipelineManager);
private: // インスタンスを受け取るポインタ
	// Command
	DirectXCommand* command_ = nullptr;
	// PipelineManager
	GraphicsPipelineManager* pipelineManager_ = nullptr;

};