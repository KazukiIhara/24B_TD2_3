#pragma once

// C++
#include <string>

// MyHedder
#include "manager/pipeline/graphics/GraphicsPipelineManager.h"
#include "math/function/MathFunction.h"
#include "structs/ObjectStructs.h"
#include "worldTransform/WorldTransform.h"

class Sprite {
public: // メンバ関数
	// 初期化
	void Initialize(const std::string& texturename, const std::string& directoryPath = "resources/images");
	// 更新
	void Update();
	// 描画
	void Draw(BlendMode blendMode = kBlendModeNormal);

	// ゲッター
	// textureHandle_
	const std::string& GetTextureFilePath()const {
		return textureFilePath_;
	}
	// 座標
	const Vector2& GetPosition()const {
		return transform.position;
	}
	// 回転
	const float& GetRotation()const {
		return transform.rotate;
	}
	// Transform
	const Transform2D& GetTransform()const {
		return transform;
	}
	// 色
	const Vector4& GetColor()const {
		return materialData_->color;
	}
	// サイズ
	const Vector2& GetSize()const {
		return transform.size;
	}
	const UVTransform& GetUVTransform()const {
		return uvTransform;
	}
	// アンカーポイント
	const Vector2& GetAnchorPoint()const {
		return anchorPoint;
	}
	// 左右フリップ
	const bool& GetIsFlipX()const {
		return isFlipX_;
	}
	// 上下フリップ
	const bool& GetIsFlipY()const {
		return isFlipY_;
	}
	// テクスチャ左上座標
	const Vector2& GetTextureLeftTop()const {
		return textureLeftTop;
	}
	// テクスチャ切り出し
	const Vector2& GetCutOutSize()const {
		return cutOutSize;
	}
	// 有効フラグ
	const bool& GetIsActive()const {
		return isActive_;
	}

	// セッター
	// textureHandle_
	void SetTextureHandle(const std::string& textureFilePath) {
		this->textureFilePath_ = textureFilePath;
	}
	// 座標
	void SetPosition(const Vector2& position) {
		this->transform.position = position;
	}
	// 回転
	void SetRotation(const float& rotation) {
		this->transform.rotate = rotation;
	}
	// 色
	void SetColor(const Vector4& color) {
		materialData_->color = color;
	}
	// サイズ
	void SetSize(const Vector2& size) {
		this->transform.size = size;
	}
	// uvTransform
	void SetUVTransform(const UVTransform& uvTransform) {
		this->uvTransform = uvTransform;
	}
	// アンカーポイント
	void SetAnchorPoint(const Vector2& anchorPoint) {
		this->anchorPoint = anchorPoint;
	}
	// 左右フリップ
	void SetIsFlipX(const bool& isFlipX) {
		this->isFlipX_ = isFlipX;
	}
	// 上下フリップ
	void SetIsFlipY(const bool& isFlipY) {
		this->isFlipY_ = isFlipY;
	}
	// テクスチャ左上座標
	void SetTextureLeftTop(const Vector2& textureLeftTop) {
		this->textureLeftTop = textureLeftTop;
	}
	// テクスチャ切り出しサイズ
	void SetCutOutSize(const Vector2& cutOutSize) {
		this->cutOutSize = cutOutSize;
	}
	// 有効フラグのセッター
	void SetIsActive(const bool& isActive) {
		this->isActive_ = isActive;
	}

private: // メンバ関数
#pragma region Vertex
	/*頂点リソースの作成*/
	void CreateVertexResource();
	/*頂点バッファビューの作成*/
	void CreateVretexBufferView();
	/*頂点データの書き込み*/
	void MapVertexData();
#pragma endregion

#pragma region Index
	/*描画用のインデックスリソースを作成*/
	void CreateIndexResource();
	/*インデックスバッファビューの作成*/
	void CreateIndexBufferView();
	/*インデックスリソースにデータを書き込む*/
	void MapIndexResource();
#pragma endregion

#pragma region Material
	/*マテリアルリソースの作成*/
	void CreateMaterialResource();
	/*マテリアルデータの書き込み*/
	void MapMaterialData();
#pragma endregion

#pragma region WVP
	/*wvp用のリソース作成*/
	void CreateWVPResource();
	/*データを書き込む*/
	void MapWVPData();
#pragma endregion


	// テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private:/*メンバ変数*/

	// TextureHandle
	std::string textureFilePath_;

	Transform2D transform{
		{1.0f,1.0f},
		0.0f,
		{0.0f,0.0f}
	};

	// アンカーポイント
	Vector2 anchorPoint = { 0.0f,0.0f };
	// 左右フリップ
	bool isFlipX_ = false;
	// 上下フリップ
	bool isFlipY_ = false;

	// 有効かどうか
	bool isActive_ = true;

	// テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	Vector2	cutOutSize = { 100.0f,100.0f };
	// uvTransform
	UVTransform uvTransform = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };

#pragma region 頂点
	/*頂点リソース*/
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	/*VBV*/
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	/*頂点データ*/
	VertexData2D* vertexData_;
#pragma endregion

#pragma region インデックス
	/*インデックスリソース*/
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	/*インデックスバッファビュー*/
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	/*インデックスデータ*/
	uint32_t* indexData_;
#pragma endregion

#pragma region マテリアル
	/*マテリアルリソース*/
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	/*マテリアルデータ*/
	Material2D* materialData_ = nullptr;
	/*マテリアル情報を受け取る箱*/
	Material2D material_;
#pragma endregion

#pragma region 変換
	/*WVP用のリソース*/
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	Matrix4x4* wvpData_;
	/*トランスフォームデータを受け取る箱*/
	WorldTransform worldTransform_;
#pragma endregion

};
