#include "UFO.h"

#include "framework/SUGER.h"

UFO::UFO() {}

UFO::~UFO() {}

void UFO::Initialize(const std::string& name) {
	EntityController::Initialize(name);
}

bool UFO::GetIsAlive() const {
	return isAlive_;
}

void UFO::Update() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case UFO::Behavior::kRoot:
			RootInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
	case UFO::Behavior::kRoot:
		RootUpdate();
		break;
	}
}

void UFO::OnCollision(Collider* other) {

}

void UFO::RootInitialize() {
	// 通常色
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void UFO::RootUpdate() {
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);

	GetCollider()->SetVelocity(velocity_);
}

void UFO::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}
