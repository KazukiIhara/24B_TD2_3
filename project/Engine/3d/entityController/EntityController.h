#pragma once

#include "3d/entity/Entity.h"

class EntityController {
public:
	EntityController() = default;
	~EntityController() = default;

	// 初期化
	virtual void Initialize(const std::string& name);

	// 更新
	virtual void Update() {};

	// コライダー作成
	void CreateCollider(const ColliderCategory& colliderCategory, const ColliderType& colliderType, const float& size);

	// 衝突コールバック関数
	virtual void OnCollision([[maybe_unused]] Collider* other);

	//
	// Setter
	//

	// スケールのセット
	void SetScale(const Vector3& scale);
	void SetScale(const float& scale);
	// 回転のセット
	void SetRotate(const Vector3& rotate);
	// トランスレートのセット
	void SetTranslate(const Vector3& translate);

	// 親トランスフォームのセット
	void SetParent(WorldTransform* parent);

	// 色のセット
	void SetColor(const Vector4& color);

	// ライトのオンオフ
	void SetEnableLight(const bool& enbleLight);

	// コライダーのトランスレートセット
	void SetColliderTranslate(const Vector3& colliderTranslate);

	//
	// Getter
	//

	// スケールの取得
	const Vector3& GetScale()const;
	// 回転の取得
	const Vector3& GetRotate()const;
	// 移動量の取得
	const Vector3& GetTranslate()const;

	// ワールドトランスフォームのポインタの取得
	WorldTransform* GetWorldTransformPtr();

	// セットされているカメラの取得
	Camera* GetCameraPtr();

	// コライダーの取得
	Collider* GetCollider();

private:
	// コントロールするエンティティ
	Entity* entity_ = nullptr;
};