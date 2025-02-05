#include "Boss.h"

#include "framework/SUGER.h"

Boss::Boss() {}

Boss::~Boss() {}

void Boss::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	SetScale(5.0f);
	SetRotateY(-std::numbers::pi_v<float> / 2.0f);
}

void Boss::Update() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
			case Boss::Behavior::kIn:
				InInitialize();
				break;
			case Boss::Behavior::kRoot:
				RootInitialize();
				break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
		case Boss::Behavior::kIn:
			InUpdate();
			break;
		case Boss::Behavior::kRoot:
			RootUpdate();
			break;
	}
}

void Boss::RequestIn() {
	behaviorRequest_ = Behavior::kIn;
}

void Boss::RequestRoot() {
	behaviorRequest_ = Behavior::kRoot;
}

void Boss::InInitialize() {

}

void Boss::InUpdate() {
	GetCollider()->SetVelocity(Vector3(-4.0f, 0.0f, 0.0f));
}

void Boss::RootInitialize() {

}

void Boss::RootUpdate() {}

void Boss::DamageInitialize() {
	hp_--;
	damageTimer_ = 0;
}

void Boss::DamageUpdate() {
	damageTimer_++;

	if (damageTimer_ == damageTime_) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Boss::AddColliderList() {
	if (behavior_ != Behavior::kNone) {
		SUGER::AddColliderList(this);
	}
}
