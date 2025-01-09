#pragma once

// C++
#include <list>

#include "directX/includes/ComPtr.h"
#include "enum/ParticleEnum.h"
#include "structs/ParticleStruct.h"
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"

class Camera;
class Model;

class Particle {
public:
	Particle() = default;
	~Particle() = default;

	// 初期化
	void Initialize(Model* model, Camera* camera, const std::string& textureFileName = "");
	// 更新
	void Update();
	// 描画
	void Draw();

	// 新規パーティクル追加
	void AddNewParticle(const EmitParamater& emitSetting);

	// カメラのセット
	void SetCamera(Camera* camera);
	// タイプのセット
	void SetType(ParticleType type);
	// ブレンドモードのセット
	void SetBlendMode(const BlendMode& blendMode);

	// 有効フラグのセッター
	void SetIsActive(const bool& isActive);

	// 有効フラグのゲッター
	const bool& GetIsActive()const;

private:
	// モデルのセット
	void SetModel(Model* model);

	// instancingリソース作成
	void CreateInstancingResource();
	// instancingリソース書き込み
	void MapInstancingData();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();
private:
	// パーティクルの最大数
	static const uint32_t kNumMaxInstance = 512;
	// パーティクル
	std::list<ParticleData> particles_;
	// パーティクルのタイプ
	ParticleType type_ = kPlane;
	// ブレンドモード(デフォルトは加算合成)
	BlendMode blendMode_ = kBlendModeAdd;

	// モデルを受け取る箱
	Model* model_ = nullptr;

	// カメラを受け取る箱
	Camera* camera_ = nullptr;

	// 板ポリ描画時のテクスチャファイル名
	std::string textureFileName_ = "";

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	ParticleForGPU* instancingData_ = nullptr;

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	// マテリアルデータ
	Material3D* materialData_;
	// マテリアル
	Material3D material_;

	// instance描画する際に使う変数
	uint32_t instanceCount_ = kNumMaxInstance;
	// SrvIndex
	uint32_t srvIndex_ = 0;

	// ストレッチビルボード作成時のスケール
	float stretchScale_ = 0.2f;

	// 有効フラグ
	bool isActive_ = true;

	// デルタタイムを設定。ひとまず60fps固定
	const float kDeltaTime = 1.0f / 60.0f;
};