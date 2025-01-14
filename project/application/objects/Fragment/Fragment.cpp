#include "Fragment.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"

void Fragment::Initialize(const std::string& name) {
	EntityController::Initialize(name);


	playerHitTimer_ = 0;
}

void Fragment::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void Fragment::Update() {

	HitTimerUpdate();

	BehaviorChange();
	BehaviorUpdate();
}

void Fragment::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
	case ColliderCategory::Player:
		if (playerHitTimer_ > 0) {
			break;
		}
		{
			float fragmentMass = GetCollider()->GetMass();
			Vector3 fragmentVelocity = GetCollider()->GetVelocity();
			float playerMass = other->GetMass();
			Vector3 playerVelocity = other->GetVelocity();
			Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
			Vector3 velocity = ComputeCollisionVelocity(fragmentMass, fragmentVelocity, playerMass, playerVelocity, 10.0f, normal);
			velocity_ = velocity;
			playerHitTimer_ = kNoneHitTime_;
		}
		break;
	}
}

void Fragment::HitTimerUpdate() {
	if (playerHitTimer_ > 0) {
		playerHitTimer_--;
	}
}

void Fragment::BehaviorChange() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Fragment::Behavior::kRoot:
			RootInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}
}

void Fragment::BehaviorUpdate() {
	// ふるまい
	switch (behavior_) {
	case Fragment::Behavior::kRoot:
		RootUpdate();
		break;
	}
}

void Fragment::RootInitialize() {
}

void Fragment::RootUpdate() {
	//Vector3 target = ExtractionWorldPos(earth_->GetWorldTransformPtr()->worldMatrix_);
	//// 目標に対して保管移動
	//SetTranslate(Lerp(GetTranslate(), target, speed_ * SUGER::kDeltaTime_));
	//SetRotate(GetRotate() + Vector3(speed_, speed_, speed_ * SUGER::kDeltaTime_));

	Move();
}

void Fragment::Move() {
	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// 現在の移動量をセット
	GetCollider()->SetVelocity(velocity_);
}

void Fragment::SetSpeed(float speed) {
	speed_ = speed;
}
