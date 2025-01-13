#include "Meteorite.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"

void Meteorite::Initialize(const std::string& name) {
	EntityController::Initialize(name);



}

void Meteorite::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void Meteorite::Update() {

	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
			case Meteorite::Behavior::kRoot:
				RootInitialize();
				break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
		case Meteorite::Behavior::kRoot:
			RootUpdate();
			break;
	}

}

void Meteorite::OnCollision(Collider* other) {

}

void Meteorite::RootInitialize() {

}

void Meteorite::RootUpdate() {
	Vector3 target = ExtractionWorldPos(earth_->GetWorldTransformPtr()->worldMatrix_);
}
