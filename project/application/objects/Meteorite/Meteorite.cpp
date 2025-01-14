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
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
	case ColliderCategory::None:

		break;
	}

}

void Meteorite::RootInitialize() {

}

void Meteorite::RootUpdate() {
	Vector3 target = ExtractionWorldPos(earth_->GetWorldTransformPtr()->worldMatrix_);
	// 目標に対して保管移動
	SetTranslate(Lerp(GetTranslate(), target, speed_ * SUGER::kDeltaTime_));
	SetRotate(GetRotate() + Vector3(speed_, speed_, speed_ * SUGER::kDeltaTime_));
}

void Meteorite::SetSpeed(float speed) {
	speed_ = speed;
}
