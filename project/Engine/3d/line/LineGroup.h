#pragma once

// C++
#include <vector>

#include "directX/includes/ComPtr.h"
#include "structs/ObjectStructs.h"
#include "structs/CameraStruct.h"
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"

class Camera;

class LineGroup {
public:
	LineGroup() = default;
	~LineGroup() = default;

	// 初期化
	void Initialize(Camera* camera);
	// 更新
	void Update();
	// 描画
	void Draw();

	// 描画するライン追加
	void AddLine(const Vector3& start, const Vector3& end, const Vector4& color);

	// ラインのクリア
	void ClearLines();

	// カメラのセット
	void SetCamera(Camera* camera);

	// 有効フラグのセッター
	void SetIsActive(const bool& isActive);
private:
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingデータ書き込み
	void MapInstancingData();

	// ViewProjectionリソース作成
	void CreateViewProjectionResource();
	// ViewProjectionデータ書き込み
	void MapViewProjectionData();
private:
	// ラインの最大数
	static const uint32_t kNumMaxInstance = 4096;
	// ライン
	std::vector<LineData3D> lines_;
	// ブレンドモード
	BlendMode blendMode_ = kBlendModeNormal;

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	LineData3D* instancingData_ = nullptr;

	// ViewProjection用のリソース
	ComPtr<ID3D12Resource> viewProjectionResource_ = nullptr;
	// シェーダーに送るWVPデータ
	ViewProjectionForGPU* viewProjectionData_ = nullptr;

	// SrvIndex
	uint32_t srvIndex_ = 0;
	// カメラを受け取る箱
	Camera* camera_ = nullptr;
	// instance描画する際に使う変数
	uint32_t instanceCount_ = kNumMaxInstance;
	// 有効フラグ
	bool isActive_ = true;
};