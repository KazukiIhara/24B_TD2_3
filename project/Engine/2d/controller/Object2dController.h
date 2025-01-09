#pragma once

// C++
#include <string>

// 数学関数ヘッダー
#include "math/function/MathFunction.h"
#include "structs/ObjectStructs.h"

#include "2d/sprite/Sprite.h"


class Object2DController {
public:
	Object2DController() = default;
	~Object2DController() = default;

	// 初期化
	void Initialize(const std::string& objectName);

	// 
	// セッター
	// 

	// 座標のセット(スクリーン座標系)
	void SetPosition(const Vector2& position);

	// 回転のセット
	void SetRotation(const float& rotation);

	// サイズのセット(pixel単位)
	void SetSize(const Vector2& size);

	// uvトランスフォームのセット
	void SetUVTransform(const UVTransform& uvTransform);

	// アンカーポイントのセット(左上{0.0f,0.0f},右下{1.0f,1.0f})
	void SetAnchorPoint(const Vector2& anchorPoint);

	// 左右フリップ
	void SetIsFlipX(const bool& isFlip);

	// 上下フリップ
	void SetIsFlipY(const bool& isFlipY);

	// テクスチャ左上座標のセット(pixel単位)
	void SetLeftTop(const Vector2& leftTop);

	// テクスチャ切り出しサイズのセット
	void SetCutOutSize(const Vector2& cutOutSize);

	// 色のセット(R,G,B,A)
	void SetColor(const Vector4& color);

	// 有効フラグのセッター
	void SetIsActive(const bool& isActive);


	// 
	// ゲッター
	// 

	// 座標の取得(スクリーン座標系)
	const Vector2& GetPosition() const;

	// 回転の取得
	const float& GetRotation() const;

	// サイズの取得(pixel単位)
	const Vector2& GetSize() const;

	// uvトランスフォームの取得
	const UVTransform& GetUVTransform() const;

	// アンカーポイントの取得(左上{0.0f,0.0f},右下{1.0f,1.0f})
	const Vector2& GetAnchorPoint() const;

	// 左右フリップの取得
	bool GetIsFlipX() const;

	// 上下フリップの取得
	bool GetIsFlipY() const;

	// テクスチャ左上座標の取得(pixel単位)
	const Vector2& GetLeftTop() const;

	// テクスチャ切り出しサイズの取得
	const Vector2& GetCutOutSize() const;

	// 色の取得(R,G,B,A)
	const Vector4& GetColor() const;

private:
	// コントロールするオブジェクト
	Sprite* object2d_ = nullptr;
};