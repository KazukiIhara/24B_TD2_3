// This
#include "Camera.h"

// C++
#include <cassert>

// ImGui
#include "externals/imgui/imgui.h"

// MyHedder
#include "framework/SUGER.h"

void Camera::Initialize() {

	transform_.Initialize();
	transform_.rotate_ = kDefaultCameraRotate_;
	transform_.translate_ = kDefaultCameraTranslate_;

	transform_.Update();
	Matrix4x4 viewMatrix = Inverse(transform_.worldMatrix_);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY_, aspectRaito_, nearClipRange_, farClipRange_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix;
	worldPos_ = ExtractionWorldPos(transform_.worldMatrix_);

	backFrontMatrix_ = MakeRotateYMatrix(std::numbers::pi_v<float>);

	CreateCameraResource();
	MapCameraData();
}

void Camera::Update() {
	transform_.Update();
	Matrix4x4 viewMatrix = Inverse(transform_.worldMatrix_);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY_, aspectRaito_, nearClipRange_, farClipRange_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix;
	worldPos_ = ExtractionWorldPos(transform_.worldMatrix_);
	billboardMatrix_ = backFrontMatrix_ * transform_.worldMatrix_;
	// 平行移動成分を削除
	billboardMatrix_.m[3][0] = 0.0f;
	billboardMatrix_.m[3][1] = 0.0f;
	billboardMatrix_.m[3][2] = 0.0f;

	UpdateCameraData();
}

void Camera::CreateCameraResource() {
	cameraResource_ = SUGER::CreateBufferResource(sizeof(CameraForGPU));
}

void Camera::MapCameraData() {
	// データを書き込む
	cameraData_ = nullptr;
	// 書き込むためのアドレスを取得
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	// カメラ
	cameraData_->worldPosition.x = worldPos_.x;
	cameraData_->worldPosition.y = worldPos_.y;
	cameraData_->worldPosition.z = worldPos_.z;
}

void Camera::UpdateCameraData() {
	cameraData_->worldPosition.x = worldPos_.x;
	cameraData_->worldPosition.y = worldPos_.y;
	cameraData_->worldPosition.z = worldPos_.z;
}

void Camera::TransferCamera(const uint32_t& index) {
	// 定数バッファを転送
	SUGER::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(index, cameraResource_->GetGPUVirtualAddress());
}

Vector3 Camera::GetTranslate() const {
	return transform_.translate_;
}

Vector3 Camera::GetRotate() const {
	return transform_.rotate_;
}

void Camera::SetTranslate(const Vector3& translate) {
	this->transform_.translate_ = translate;
}

void Camera::SetRotate(const Vector3& rotate) {
	this->transform_.rotate_ = rotate;
}

WorldTransform* Camera::GetWorldTransformPtr() {
	return &transform_;
}

Matrix4x4* Camera::GetViewProjectionMatrixPtr() {
	return &viewProjectionMatrix_;
}

Matrix4x4 Camera::GetViewProjectionMatrix() const {
	return viewProjectionMatrix_;
}

Matrix4x4 Camera::GetWorldMatrix() const {
	return transform_.worldMatrix_;
}

Vector3 Camera::GetWorldPos() const {
	return worldPos_;
}

Matrix4x4 Camera::GetBillboardMatrix() const {
	return billboardMatrix_;
}

