#include "Explosion.h"

#include "framework/SUGER.h"

void Explosion::Initialize() {

	// 必要なエミッターとパーティクル作成
	SUGER::CreateParticle("fire", kPlane, "circle.png");
	SUGER::CreateParticle("flash", kPlane, "circle.png");

	// fire
	fireEmitter_.Initialize(SUGER::CreateEmitter("fire"));
	fireEmitter_.SetParticle("fire");
	fireEmitter_.SetIsRepeat(false);

	fire.emitType = kRandom;
	fire.minColor = { 0.0f,0.0f,0.2f };
	fire.maxColor = { 0.0f,0.1f,1.0f };
	fire.minScale = 0.1f;
	fire.maxScale = 0.75f;
	fire.minTranslate = { -0.1f, -0.8f,0.0f };
	fire.maxTranslate = { 0.01f,0.4f,0.0f };
	fire.minVelocity = { -0.1f,0.1f,0.0f };
	fire.maxVelocity = { 0.1f,0.8f,0.0f };
	fire.minLifeTime = 0.2f;
	fire.maxLifeTime = 1.7f;
	fire.count = 100;
	fire.isRepeat = false;
	fire.frequency = 0.0f;

	fireEmitter_.SetEmitterSetting(fire);

	// fire2
	fireEmitter2_.Initialize(SUGER::CreateEmitter("fire"));
	fireEmitter2_.SetParticle("fire");
	fireEmitter2_.SetIsRepeat(false);

	fire2.emitType = kRandom;
	fire2.minColor = { 0.0f,0.0f,0.2f };
	fire2.maxColor = { 0.0f,0.1f,1.0f };
	fire2.minScale = 0.2f;
	fire2.maxScale = 0.9f;
	fire2.minTranslate = { -0.23f,-0.8f,0.0f };
	fire2.maxTranslate = { 0.36f,0.0f,0.0f };
	fire2.minVelocity = { -0.1f,0.1f,0.0f };
	fire2.maxVelocity = { 0.1f,0.8f,0.0f };
	fire2.minLifeTime = 0.2f;
	fire2.maxLifeTime = 1.5f;
	fire2.count = 100;
	fire2.isRepeat = false;
	fire2.frequency = 0.0f;

	fireEmitter2_.SetEmitterSetting(fire2);

	// flash
	flashEmitter_.Initialize(SUGER::CreateEmitter("flash"));
	flashEmitter_.SetParticle("flash");
	flashEmitter_.SetIsRepeat(false);

	flash.emitType = kRadialZ;
	flash.minColor = { 0.0f,0.0f,0.0f };
	flash.maxColor = { 0.0f,0.0f,1.0f };
	flash.minScale = 0.05f;
	flash.maxScale = 0.05f;
	flash.minTranslate = { 0.0f,0.0f,-1.0f };
	flash.maxTranslate = { 0.0f,0.0f,-1.0f };
	flash.speed = 4.0f;
	flash.minLifeTime = 2.0f;
	flash.maxLifeTime = 2.0f;
	flash.count = 256;
	flash.isRepeat = false;
	flash.frequency = 0.0f;

	flashEmitter_.SetEmitterSetting(flash);
}

void Explosion::Update() {
	fireEmitter_.SetEmitterSetting(fire);
	fireEmitter2_.SetEmitterSetting(fire2);
	flashEmitter_.SetEmitterSetting(flash);
}

void Explosion::Emit() {
	fireEmitter_.Emit();
	fireEmitter2_.Emit();
	flashEmitter_.Emit();

}

void Explosion::ShowEmitterSettingsUI(EmitterSetting& emitterSetting) {
	if (ImGui::Begin("EmitterSettings")) {
		// 色設定
		ImGui::Text("Color");
		ImGui::ColorEdit3("Min Color", reinterpret_cast<float*>(&emitterSetting.minColor));
		ImGui::ColorEdit3("Max Color", reinterpret_cast<float*>(&emitterSetting.maxColor));

		// サイズ設定
		ImGui::Text("Size");
		ImGui::DragFloat("Min Size", &emitterSetting.minScale, 0.01f, 0.0f, 10.0f, "%.2f");
		ImGui::DragFloat("Max Size", &emitterSetting.maxScale, 0.01f, 0.0f, 10.0f, "%.2f");

		// 座標範囲設定
		ImGui::Text("Translation Range");
		ImGui::DragFloat3("Min Translate", reinterpret_cast<float*>(&emitterSetting.minTranslate), 0.01f, -10.0f, 10.0f, "%.2f");
		ImGui::DragFloat3("Max Translate", reinterpret_cast<float*>(&emitterSetting.maxTranslate), 0.01f, -10.0f, 10.0f, "%.2f");

		// 移動量設定
		ImGui::Text("Velocity Range");
		ImGui::DragFloat3("Min Velocity", reinterpret_cast<float*>(&emitterSetting.minVelocity), 0.01f, -1.0f, 1.0f, "%.2f");
		ImGui::DragFloat3("Max Velocity", reinterpret_cast<float*>(&emitterSetting.maxVelocity), 0.01f, -1.0f, 1.0f, "%.2f");

		// 生存時間設定
		ImGui::Text("Life Time");
		ImGui::DragFloat("Min Life Time", &emitterSetting.minLifeTime, 0.01f, 0.0f, 10.0f, "%.2f");
		ImGui::DragFloat("Max Life Time", &emitterSetting.maxLifeTime, 0.01f, 0.0f, 10.0f, "%.2f");

		// 発生数設定
		ImGui::Text("Particle Count");
		ImGui::DragInt("Count", reinterpret_cast<int*>(&emitterSetting.count), 1, 1, 1000);

		ImGui::End();
	}
}