#pragma once

// C++
#include <string>
#include <memory>

// MyHedder
#include "worldTransform/WorldTransform.h"
#include "structs/ObjectStructs.h"

// モデルなしの3Dオブジェクト
class Empty {
public:
	Empty() = default;
	~Empty() = default;

	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();

	//
	// Setter
	//

	// 名前のセット
	void SetName(const std::string& name);

	// スケールのセット
	void SetScale(const Vector3& scale);
	// 回転のセット
	void SetRotate(const Vector3& rotate);
	// 移動量のセット
	void SetTranslate(const Vector3& translate);

	// トランスフォームのセット
	void SetTransform(const EulerTransform3D& transform);

	// 親トランスフォームのセット
	void SetParent(WorldTransform* parent);

	// 有効フラグのセット
	void SetIsActive(const bool& isActive);

	// 削除フラグのセット
	void SetIsDelete(const bool& isDelete);

	//
	// Getter
	//

	// 名前のゲット
	const std::string& GetName()const;

	// スケールのゲッター
	const Vector3& GetScale()const;
	// 回転のゲッター
	const Vector3& GetRotate()const;
	// 移動量のゲッター
	const Vector3& GetTranslate()const;

	// ワールド座標のゲッター
	Vector3 GetWorldPosition()const;

	// ワールドトランスフォームのポインタのゲッター
	WorldTransform* GetWorldTransformPtr();


	// 有効フラグのゲット
	const bool& GetIsActive()const;

	// 削除フラグのゲット
	const bool& GetIsDelete()const;

protected:
	// 名前
	std::string name_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 有効フラグ
	bool isActive_ = true;
	// 削除フラグ
	bool isDelete_ = false;
};