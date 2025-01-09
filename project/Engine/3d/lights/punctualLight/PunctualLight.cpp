#include "PunctualLight.h"

// MyHedder
#include "framework/SUGER.h"

void PunctualLight::Initialize() {
	// ライトの状態を初期化
	InitializeLightSetting();
	// リソース作成
	CreatePunctualLightResource();
	// データ書き込み
	MapPunctualLightData();
}

void PunctualLight::Update() {
	UpdateDirectionalLight();
	UpdatePointLight();
	UpdateSpotLight();
}

void PunctualLight::TransferLight() {
	// 定数バッファを転送
	SUGER::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(2, punctualLightResource_->GetGPUVirtualAddress());
}

void PunctualLight::InitializeLightSetting() {
	// DirectionalLightの初期化
	punctualLight.directionalLight.color = { 1.0f,1.0f,1.0f,1.0f };
	punctualLight.directionalLight.direction = { 0.0f,-1.0f,0.0f };
	punctualLight.directionalLight.intensity = 1.0f;

	// PointLightの初期化
	punctualLight.pointLight.color = { 1.0f,1.0f,1.0f,1.0f };
	punctualLight.pointLight.intensity = 0.0f;
	punctualLight.pointLight.position = { 0.0f,2.0f,7.0f };
	punctualLight.pointLight.radius = 10.0f;
	punctualLight.pointLight.decay = 5.0f;

	// SpotLightの初期化
	punctualLight.spotLight.color = { 1.0f,1.0f,1.0f,1.0f };
	punctualLight.spotLight.position = { 0.0f,2.0f,10.0f };
	punctualLight.spotLight.distance = 7.0f;
	punctualLight.spotLight.direction = Normalize(Vector3(0.0f, -1.0f, 0.0f));
	punctualLight.spotLight.intensity = 0.0f;
	punctualLight.spotLight.decay = 2.0f;
	punctualLight.spotLight.cosFalloffStart = 0.1f;
	punctualLight.spotLight.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
}

void PunctualLight::SetPunctualLightSetting(const PunctualLightForGPU& punctualLightSetting) {
	punctualLight = punctualLightSetting;
}

void PunctualLight::CreatePunctualLightResource() {
	// WVP用のリソースを作る
	punctualLightResource_ = SUGER::CreateBufferResource(sizeof(PunctualLightForGPU));
}

void PunctualLight::MapPunctualLightData() {
	// データを書き込む
	punctualLightData_ = nullptr;
	// 書き込むためのアドレスを取得
	punctualLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&punctualLightData_));

	// DirectionalLight
	punctualLightData_->directionalLight.color = punctualLight.directionalLight.color;
	punctualLightData_->directionalLight.direction = punctualLight.directionalLight.direction;
	punctualLightData_->directionalLight.intensity = punctualLight.directionalLight.intensity;

	// PointLight
	punctualLightData_->pointLight.color = punctualLight.pointLight.color;
	punctualLightData_->pointLight.decay = punctualLight.pointLight.decay;
	punctualLightData_->pointLight.intensity = punctualLight.pointLight.intensity;
	punctualLightData_->pointLight.position = punctualLight.pointLight.position;
	punctualLightData_->pointLight.radius = punctualLight.pointLight.radius;

	// SpotLight
	punctualLightData_->spotLight.color = punctualLight.spotLight.color;
	punctualLightData_->spotLight.cosAngle = punctualLight.spotLight.cosAngle;
	punctualLightData_->spotLight.cosFalloffStart = punctualLight.spotLight.cosFalloffStart;
	punctualLightData_->spotLight.decay = punctualLight.spotLight.decay;
	punctualLightData_->spotLight.direction = punctualLight.spotLight.direction;
	punctualLightData_->spotLight.distance = punctualLight.spotLight.distance;
	punctualLightData_->spotLight.intensity = punctualLight.spotLight.intensity;
	punctualLightData_->spotLight.position = punctualLight.spotLight.position;

}

void PunctualLight::UpdateDirectionalLight() {
	punctualLightData_->directionalLight.color = punctualLight.directionalLight.color;
	punctualLightData_->directionalLight.direction = punctualLight.directionalLight.direction;
	punctualLightData_->directionalLight.intensity = punctualLight.directionalLight.intensity;
}

void PunctualLight::UpdatePointLight() {
	punctualLightData_->pointLight.color = punctualLight.pointLight.color;
	punctualLightData_->pointLight.decay = punctualLight.pointLight.decay;
	punctualLightData_->pointLight.intensity = punctualLight.pointLight.intensity;
	punctualLightData_->pointLight.position = punctualLight.pointLight.position;
	punctualLightData_->pointLight.radius = punctualLight.pointLight.radius;
}

void PunctualLight::UpdateSpotLight() {
	punctualLightData_->spotLight.color = punctualLight.spotLight.color;
	punctualLightData_->spotLight.cosAngle = punctualLight.spotLight.cosAngle;
	punctualLightData_->spotLight.cosFalloffStart = punctualLight.spotLight.cosFalloffStart;
	punctualLightData_->spotLight.decay = punctualLight.spotLight.decay;
	punctualLightData_->spotLight.direction = punctualLight.spotLight.direction;
	punctualLightData_->spotLight.distance = punctualLight.spotLight.distance;
	punctualLightData_->spotLight.intensity = punctualLight.spotLight.intensity;
	punctualLightData_->spotLight.position = punctualLight.spotLight.position;
}