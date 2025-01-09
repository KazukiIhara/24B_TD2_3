#pragma once

// 前方宣言
class DirectXCommand;
class GraphicsPipelineManager;

class Object3DSystem {
public: // 公開メンバ関数
	Object3DSystem() = default;
	~Object3DSystem() = default;

	// 初期化処理
	void Initialize(DirectXCommand* command, GraphicsPipelineManager* pipelineManager);

	// 3dオブジェクト描画前処理
	void PreDraw();

	// スキニングあり描画前処理
	void PreDrawSkinning();
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