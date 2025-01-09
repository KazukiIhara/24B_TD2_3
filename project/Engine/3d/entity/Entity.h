#pragma once

#include "directX/includes/ComPtr.h"
#include "3d/model/Model.h"
#include "3d/cameras/camera/Camera.h"
#include "3d/lights/punctualLight/PunctualLight.h"
#include "3d/empty/empty.h"
#include "collider/Collider.h"

#include "enum/GraphicsPipelineEnum.h"
#include "enum/BlendModeEnum.h"

// モデルつきの3Dオブジェクト
class Entity :public Empty {
public:
	Entity() = default;
	~Entity() = default;

	// 初期化
	void Initialize()override;
	// 更新
	virtual void Update()override;
	// 描画
	virtual void Draw();

	// コライダー作成
	void CreateCollider(const ColliderCategory& colliderCategory, const ColliderType& colliderType, const float& size);

	// コライダーの取得
	Collider* GetCollider();

	// セットされているカメラを取得
	Camera* GetCamera();

	// modelのセット
	void SetModel(const std::string& filePath);
	// カメラのセット
	void SetCamera(Camera* camera);
	// ライトのセット
	void SetLight(PunctualLight* punctualLight);

	// 色をセット
	void SetColor(const Vector4& color);
	// ライトオンオフ
	void SetEnableLight(const bool& enableLight);
	// 光沢のセット
	void SetShininess(const float& shininess);

private:
	// WVP用のリソース作成
	void CreateWVPResource();
	// データを書き込む
	void MapWVPData();
	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMateiralData();
protected:
	// モデルを受け取る箱
	Model* model_ = nullptr;

	// コライダー
	std::unique_ptr<Collider> collider_;

	// ブレンドモード
	BlendMode blendMode_ = kBlendModeNormal;

	// WVP用のリソース
	ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	// シェーダーに送るWVPデータ
	TransformationMatrix* transformationData_ = nullptr;

	// カメラを受け取る箱
	Camera* camera_ = nullptr;
	// ライトを受け取る箱
	PunctualLight* punctualLight_ = nullptr;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material3D* materialData_ = nullptr;
	// マテリアル
	Material3D material_;

};
