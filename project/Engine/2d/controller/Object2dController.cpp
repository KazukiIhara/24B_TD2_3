#include "Object2dController.h"

#include <cassert>

#include "framework/SUGER.h"

void Object2DController::Initialize(const std::string& objectName) {
	// Spriteをマネージャから持ってきてセット
	object2d_ = SUGER::FindObject2D(objectName);
	// なかったらassert
	assert(object2d_);
}


void Object2DController::SetPosition(const Vector2& position) {
	// 初期化時にチェック済みのため、nullチェックは不要
	object2d_->SetPosition(position);
}

void Object2DController::SetRotation(const float& rotation) {
	object2d_->SetRotation(rotation);
}

void Object2DController::SetSize(const Vector2& size) {
	object2d_->SetSize(size);
}

void Object2DController::SetUVTransform(const UVTransform& uvTransform) {
	object2d_->SetUVTransform(uvTransform);
}

void Object2DController::SetAnchorPoint(const Vector2& anchorPoint) {
	object2d_->SetAnchorPoint(anchorPoint);
}

void Object2DController::SetIsFlipX(const bool& isFlip) {
	object2d_->SetIsFlipX(isFlip);
}

void Object2DController::SetIsFlipY(const bool& isFlipY) {
	object2d_->SetIsFlipY(isFlipY);
}

void Object2DController::SetLeftTop(const Vector2& leftTop) {
	object2d_->SetTextureLeftTop(leftTop);
}

void Object2DController::SetCutOutSize(const Vector2& cutOutSize) {
	object2d_->SetCutOutSize(cutOutSize);
}

void Object2DController::SetColor(const Vector4& color) {
	object2d_->SetColor(color);
}
void Object2DController::SetIsActive(const bool& isActive) {
	object2d_->SetIsActive(isActive);
}

// 座標の取得(スクリーン座標系)
const Vector2& Object2DController::GetPosition() const {
	return object2d_->GetPosition();
}

// 回転の取得
const float& Object2DController::GetRotation() const {
	return object2d_->GetRotation();
}

// サイズの取得(pixel単位)
const Vector2& Object2DController::GetSize() const {
	return object2d_->GetSize();
}

// uvトランスフォームの取得
const UVTransform& Object2DController::GetUVTransform() const {
	return object2d_->GetUVTransform();
}

// アンカーポイントの取得(左上{0.0f,0.0f},右下{1.0f,1.0f})
const Vector2& Object2DController::GetAnchorPoint() const {
	return object2d_->GetAnchorPoint();
}

// 左右フリップの取得
bool Object2DController::GetIsFlipX() const {
	return object2d_->GetIsFlipX();
}

// 上下フリップの取得
bool Object2DController::GetIsFlipY() const {
	return object2d_->GetIsFlipY();
}

// テクスチャ左上座標の取得(pixel単位)
const Vector2& Object2DController::GetLeftTop() const {
	return object2d_->GetTextureLeftTop();
}

// テクスチャ切り出しサイズの取得
const Vector2& Object2DController::GetCutOutSize() const {
	return object2d_->GetCutOutSize();
}

// 色の取得(R,G,B,A)
const Vector4& Object2DController::GetColor() const {
	return object2d_->GetColor();
}
