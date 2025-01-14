#include "Meteorite.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"

#include "system/FragmentManager/FragmentManager.h"

void Meteorite::Initialize(const std::string& name) {
	EntityController::Initialize(name);


}

void Meteorite::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void Meteorite::SetFragmentManager(FragmentManager* fragmentManager){
	assert(fragmentManager);
	fragmentManager_ = fragmentManager;
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
	Vector3 velocity = Lerp(GetTranslate(), target, speed_) - GetTranslate();
	SetTranslate(GetTranslate() + velocity * SUGER::kDeltaTime_);
	SetRotate(GetRotate() + Vector3(speed_, speed_, speed_ * SUGER::kDeltaTime_));

	GetCollider()->SetVelocity(velocity);


	emitTime_++;

	if (emitTime_ >= 300) {
		Vector3 popPos = GetTranslate();
		popPos.x += rand() % 21 + -10;
		popPos.y += rand() % 21 + -10;
		fragmentManager_->AddFragment(popPos,earth_);
		emitTime_ = 0;
	}


}

void Meteorite::SetSpeed(float speed) {
	speed_ = speed;
}
