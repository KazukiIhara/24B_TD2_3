#include "empty.h"

void Empty::Initialize() {
	// 有効化
	isActive_ = true;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Empty::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.Update();
}

void Empty::SetName(const std::string& name) {
	name_ = name;
}

void Empty::SetScale(const Vector3& scale) {
	worldTransform_.scale_ = scale;
}

void Empty::SetRotate(const Vector3& rotate) {
	worldTransform_.rotate_ = rotate;
}

void Empty::SetTranslate(const Vector3& translate) {
	worldTransform_.translate_ = translate;
}

void Empty::SetTransform(const EulerTransform3D& transform) {
	worldTransform_.scale_ = transform.scale;
	worldTransform_.rotate_ = transform.rotate;
	worldTransform_.translate_ = transform.translate;
}

void Empty::SetParent(WorldTransform* parent) {
	worldTransform_.parent_ = parent;
}

void Empty::SetIsActive(const bool& isActive) {
	isActive_ = isActive;
}

void Empty::SetIsDelete(const bool& isDelete) {
	isDelete_ = isDelete;
}

const std::string& Empty::GetName() const {
	return name_;
}

const Vector3& Empty::GetScale() const {
	return worldTransform_.scale_;
}

const Vector3& Empty::GetRotate() const {
	return worldTransform_.rotate_;
}

const Vector3& Empty::GetTranslate() const {
	return worldTransform_.translate_;
}

Vector3 Empty::GetWorldPosition()const {
	return ExtractionWorldPos(worldTransform_.worldMatrix_);
}

WorldTransform* Empty::GetWorldTransformPtr() {
	return &worldTransform_;
}

const bool& Empty::GetIsActive() const {
	return isActive_;
}

const bool& Empty::GetIsDelete() const {
	return isDelete_;
}
