#include "WorldTransform.h"

void WorldTransform::Initialize() {
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	translate_ = { 0.0f,0.0f,0.0f };
	worldMatrix_ = MakeIdentityMatrix4x4();
}

void WorldTransform::Update() {
	// アフィン行列更新
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	// 親がいれば
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}
}