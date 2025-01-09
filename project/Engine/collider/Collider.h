#pragma once

#include "math/function/MathFunction.h"
#include "enum/ColliderEnum.h"
#include "colliderCategory/ColliderCategory.h"
#include "3d/empty/empty.h"
#include "3d/lineController/LineController.h"

class Collider:public Empty {
public:
	Collider() = default;
	~Collider() = default;

	// 初期化
	void Initialize(WorldTransform* worldTransform, const ColliderCategory& colliderCategory, const ColliderType& colliderType, const float& size);
	// 描画
	void Draw();

	// スフィアの描画
	void DrawSphere(const Vector3& center, float radius, uint32_t segments);

	// 親トランスフォームのセット
	void SetParent(WorldTransform* worldTransform);

	// コライダータイプのセッター
	void SetColliderType(const ColliderType& colliderType);

	// コライダーカテゴリのセッター
	void SetColliderCategory(const ColliderCategory& colliderCategory);

	// サイズのセット
	void SetSize(const float& size);

	// ワールド座標のゲッター
	Vector3 GetWorldPosition()const;

	// コライダータイプのゲッター
	ColliderType GetColliderType()const;

	// コライダーカテゴリーのゲッター
	ColliderCategory GetColliderCategory()const;

	// サイズの取得
	float GetSize()const;
private:
	// ラインコントローラのセット
	void SetLineController(LineController* lineController);

private:
	// サイズ
	float size_ = 1.0f;
	// コライダータイプ
	ColliderType colliderType_ = ColliderType::kSphere;
	// コライダーカテゴリ
	ColliderCategory colliderCategory_ = ColliderCategory::kNone;
	// ライン描画コントローラ
	LineController* line_;
};