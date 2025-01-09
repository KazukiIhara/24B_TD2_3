#include "EmitterController.h"

#include "framework/SUGER.h"

void EmitterController::Initialize(const std::string& name) {
	emitter_ = SUGER::FindEmitter(name);
	assert(emitter_);
}

void EmitterController::Emit() {
	emitter_->Emit();
}

void EmitterController::SetScale(const Vector3& scale) {
	emitter_->SetScale(scale);
}

void EmitterController::SetScale(const float& scale) {
	emitter_->SetScale(Vector3(scale, scale, scale));
}

void EmitterController::SetRotate(const Vector3& rotate) {
	emitter_->SetRotate(rotate);
}

void EmitterController::SetTranslate(const Vector3& translate) {
	emitter_->SetTranslate(translate);
}

void EmitterController::SetParent(WorldTransform* parent) {
	emitter_->SetParent(parent);
}

void EmitterController::SetParticle(const std::string& particleName) {
	emitter_->SetParticle(particleName);
}

void EmitterController::SetCount(const uint32_t& count) {
	emitter_->SetCount(count);
}

void EmitterController::SetIsRepeat(const bool& isRepeat) {
	emitter_->SetIsRepeat(isRepeat);
}

void EmitterController::SetFrequency(const float& frequency) {
	emitter_->SetFrequency(frequency);
}

void EmitterController::SetEmitType(const EmitType& emitType) {
	emitter_->SetEmitType(emitType);
}

void EmitterController::SetEmitterSetting(const EmitterSetting& emitterSetting) {
	emitter_->SetEmitterSetting(emitterSetting);
}

void EmitterController::SetMinPosition(const Vector3& minPosition) {
	emitter_->SetMinPosition(minPosition);
}

void EmitterController::SetMaxPosition(const Vector3& maxPosition) {
	emitter_->SetMaxPosition(maxPosition);
}

void EmitterController::SetMinSize(float minSize) {
	emitter_->SetMinSize(minSize);
}

void EmitterController::SetMaxSize(float maxSize) {
	emitter_->SetMaxSize(maxSize);
}

void EmitterController::SetMinVelocity(const Vector3& minVelocity) {
	emitter_->SetMinVelocity(minVelocity);
}

void EmitterController::SetMaxVelocity(const Vector3& maxVelocity) {
	emitter_->SetMaxVelocity(maxVelocity);
}

void EmitterController::SetMinColor(const Vector3& minColor) {
	emitter_->SetMinColor(minColor);
}

void EmitterController::SetMaxColor(const Vector3& maxColor) {
	emitter_->SetMaxColor(maxColor);
}

void EmitterController::SetMinLifeTime(float minLifeTime) {
	emitter_->SetMinLifeTime(minLifeTime);
}

void EmitterController::SetMaxLifeTime(float maxLifeTime) {
	emitter_->SetMaxLifeTime(maxLifeTime);
}

const Vector3& EmitterController::GetScale() const {
	return emitter_->GetScale();
}

const Vector3& EmitterController::GetRotate() const {
	return emitter_->GetRotate();
}

const Vector3& EmitterController::GetTranslate() const {
	return emitter_->GetTranslate();
}

Vector3 EmitterController::GetWorldPosition() const {
	return emitter_->GetWorldPosition();
}

WorldTransform* EmitterController::GetWorldTransformPtr() {
	return emitter_->GetWorldTransformPtr();
}