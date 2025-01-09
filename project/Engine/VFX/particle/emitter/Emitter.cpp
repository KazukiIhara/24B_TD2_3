#include "Emitter.h"

#include "framework/SUGER.h"
#include "random/Random.h"

void Emitter::Initialize() {
	// 基底クラスの初期化
	Empty::Initialize();

	// エミッターの設定初期化
	InitializeEmitterSetting();

	// 座標は0,0,0
	emitParamater_.position = { 0.0f,0.0f,0.0f };
	// サイズは1.0
	emitParamater_.scale = { 1.0f,1.0f,1.0f };
	// 移動量は0
	emitParamater_.velocity = { 0.0f,0.0f,0.0f };
	// 色は白
	emitParamater_.color = { 1.0f,1.0f,1.0f,1.0f };
	// 生存時間をひとまず5秒に設定
	emitParamater_.lifeTime = 5.0f;
}

void Emitter::Update() {
	// 基底クラスの更新
	Empty::Update();

	// 繰り返しフラグがオンなら繰り返し発生
	if (emitterSetting_.isRepeat) {

		// エミッターの処理
		// 時刻を進める
		frequencyTime_ += kDeltaTime_;

		// 頻度より大きいなら発生
		if (emitterSetting_.frequency <= frequencyTime_) {
			Emit();
			// 余計に過ぎた時間も加味して頻度計算する
			frequencyTime_ -= emitterSetting_.frequency;
		}
	}
}

void Emitter::Emit() {
	// パーティクル発生ポジション
	Vector3 emitterPosition = GetWorldPosition();
	emitParamater_.position = emitterPosition;
	// 発生タイプごとの処理
	switch (emitterSetting_.emitType) {
		// デフォルト
		case kDefault:
			// 発生個数分ループ
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// サイズ
				float scale = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater_.scale.x = scale;
				emitParamater_.scale.y = scale;
				emitParamater_.scale.z = scale;
				// 座標
				emitParamater_.position.x = emitterPosition.x + i * 0.1f;
				emitParamater_.position.y = emitterPosition.y + i * 0.1f;
				particle_->AddNewParticle(emitParamater_);
			}
			break;
			// ランダム 
		case kRandom:
			// 発生個数分ループ
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// 発生座標
				emitParamater_.position.x = emitterPosition.x + Random::GenerateFloat(emitterSetting_.minTranslate.x, emitterSetting_.maxTranslate.x);
				emitParamater_.position.y = emitterPosition.y + Random::GenerateFloat(emitterSetting_.minTranslate.y, emitterSetting_.maxTranslate.y);
				emitParamater_.position.z = emitterPosition.z + Random::GenerateFloat(emitterSetting_.minTranslate.z, emitterSetting_.maxTranslate.z);
				// サイズ
				float size = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater_.scale.x = size;
				emitParamater_.scale.y = size;
				emitParamater_.scale.z = size;
				// 移動量
				emitParamater_.velocity.x = Random::GenerateFloat(emitterSetting_.minVelocity.x, emitterSetting_.maxVelocity.x);
				emitParamater_.velocity.y = Random::GenerateFloat(emitterSetting_.minVelocity.y, emitterSetting_.maxVelocity.y);
				emitParamater_.velocity.z = Random::GenerateFloat(emitterSetting_.minVelocity.z, emitterSetting_.maxVelocity.z);
				// 色
				emitParamater_.color.x = Random::GenerateFloat(emitterSetting_.minColor.x, emitterSetting_.maxColor.x);
				emitParamater_.color.y = Random::GenerateFloat(emitterSetting_.minColor.y, emitterSetting_.maxColor.y);
				emitParamater_.color.z = Random::GenerateFloat(emitterSetting_.minColor.z, emitterSetting_.maxColor.z);

				// 生存時間
				emitParamater_.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particle_->AddNewParticle(emitParamater_);
			}
			break;
			// X軸方向に放射状に発生
		case kRadialX:
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// サイズ
				float size = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater_.scale.x = size;
				emitParamater_.scale.y = size;
				emitParamater_.scale.z = size;

				// 移動量
				float angle = 2.0f * std::numbers::pi_v<float> *i / emitterSetting_.count;
				emitParamater_.velocity.x = 0.0f;
				emitParamater_.velocity.y = emitterSetting_.speed * std::sin(angle);
				emitParamater_.velocity.z = emitterSetting_.speed * std::cos(angle);

				// 色
				emitParamater_.color.x = Random::GenerateFloat(emitterSetting_.minColor.x, emitterSetting_.maxColor.x);
				emitParamater_.color.y = Random::GenerateFloat(emitterSetting_.minColor.y, emitterSetting_.maxColor.y);
				emitParamater_.color.z = Random::GenerateFloat(emitterSetting_.minColor.z, emitterSetting_.maxColor.z);

				// 生存時間
				emitParamater_.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particle_->AddNewParticle(emitParamater_);
			}
			break;

			// Y軸方向に放射状に発生
		case kRadialY:
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// サイズ
				float size = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater_.scale.x = size;
				emitParamater_.scale.y = size;
				emitParamater_.scale.z = size;

				// 移動量
				float angle = 2.0f * std::numbers::pi_v<float> *i / emitterSetting_.count;
				emitParamater_.velocity.x = emitterSetting_.speed * std::cos(angle);
				emitParamater_.velocity.y = 0.0f;
				emitParamater_.velocity.z = emitterSetting_.speed * std::sin(angle);

				// 色
				emitParamater_.color.x = Random::GenerateFloat(emitterSetting_.minColor.x, emitterSetting_.maxColor.x);
				emitParamater_.color.y = Random::GenerateFloat(emitterSetting_.minColor.y, emitterSetting_.maxColor.y);
				emitParamater_.color.z = Random::GenerateFloat(emitterSetting_.minColor.z, emitterSetting_.maxColor.z);

				// 生存時間
				emitParamater_.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particle_->AddNewParticle(emitParamater_);
			}
			break;

			// Z軸方向に放射状に発生
		case kRadialZ:
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// サイズ
				float size = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater_.scale.x = size;
				emitParamater_.scale.y = size;
				emitParamater_.scale.z = size;

				// 移動量
				float angle = 2.0f * std::numbers::pi_v<float> *i / emitterSetting_.count;
				emitParamater_.velocity.x = emitterSetting_.speed * std::cos(angle);
				emitParamater_.velocity.y = emitterSetting_.speed * std::sin(angle);
				emitParamater_.velocity.z = 0.0f;

				// 色
				emitParamater_.color.x = Random::GenerateFloat(emitterSetting_.minColor.x, emitterSetting_.maxColor.x);
				emitParamater_.color.y = Random::GenerateFloat(emitterSetting_.minColor.y, emitterSetting_.maxColor.y);
				emitParamater_.color.z = Random::GenerateFloat(emitterSetting_.minColor.z, emitterSetting_.maxColor.z);

				// 生存時間
				emitParamater_.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particle_->AddNewParticle(emitParamater_);
			}
			break;
	}
}

void Emitter::SetParticle(const std::string& particleName) {
	// パーティクルがなければassert
	particle_ = SUGER::FindParticle(particleName);
}

void Emitter::InitializeEmitterSetting() {
	// 発生設定の初期化
	emitterSetting_.count = 1;
	emitterSetting_.emitType = kDefault;
	emitterSetting_.frequency = 1.0f;
	emitterSetting_.isRepeat = false;
	emitterSetting_.minColor = { 1.0f,1.0f,1.0f };
	emitterSetting_.maxColor = { 1.0f,1.0f,1.0f };
	emitterSetting_.minScale = 1.0f;
	emitterSetting_.maxScale = 1.0f;
	emitterSetting_.minVelocity = { 0.0f,0.0f,0.0f };
	emitterSetting_.maxVelocity = { 0.0f,0.0f,0.0f };
	emitterSetting_.minTranslate = { 0.0f,0.0f,0.0f };
	emitterSetting_.maxTranslate = { 0.0f,0.0f,0.0f };
	emitterSetting_.minLifeTime = 1.0f;
	emitterSetting_.maxLifeTime = 1.0f;
}

void Emitter::SetCount(const uint32_t& count) {
	emitterSetting_.count = count;
}

void Emitter::SetFrequency(const float& frequency) {
	emitterSetting_.frequency = frequency;
}

void Emitter::SetIsRepeat(const bool& isRepeat) {
	emitterSetting_.isRepeat = isRepeat;
}

void Emitter::SetEmitType(const EmitType& emitType) {
	emitterSetting_.emitType = emitType;
}

void Emitter::SetEmitterSetting(const EmitterSetting& emitterSetting) {
	emitterSetting_ = emitterSetting;
}

void Emitter::SetMinSize(float minSize) {
	emitterSetting_.minScale = minSize;
}

void Emitter::SetMaxSize(float maxSize) {
	emitterSetting_.maxScale = maxSize;
}

void Emitter::SetMinPosition(const Vector3& minPosition) {
	emitterSetting_.minTranslate = minPosition;
}

void Emitter::SetMaxPosition(const Vector3& maxPosition) {
	emitterSetting_.maxTranslate = maxPosition;
}

void Emitter::SetMinVelocity(const Vector3& minVelocity) {
	emitterSetting_.minVelocity = minVelocity;
}

void Emitter::SetMaxVelocity(const Vector3& maxVelocity) {
	emitterSetting_.maxVelocity = maxVelocity;
}

void Emitter::SetMinColor(const Vector3& minColor) {
	emitterSetting_.minColor = minColor;
}

void Emitter::SetMaxColor(const Vector3& maxColor) {
	emitterSetting_.maxColor = maxColor;
}

void Emitter::SetMinLifeTime(float minLifeTime) {
	emitterSetting_.minLifeTime = minLifeTime;
}

void Emitter::SetMaxLifeTime(float maxLifeTime) {
	emitterSetting_.maxLifeTime = maxLifeTime;
}