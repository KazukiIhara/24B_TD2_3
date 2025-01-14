#include "Earth.h"

#include "framework/SUGER.h"

void Earth::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	hitTimer_ = 0;
}

void Earth::Update() {
	if (hitTimer_ > 0) {
		hitTimer_--;
	}

	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// コライダーに移動量をセット
	GetCollider()->SetVelocity(velocity_);
}

void Earth::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
	case ColliderCategory::Player:
		if (hitTimer_ > 0) {
			break;
		}
		float earthMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		float playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 normal = GetCollider()->GetWorldPosition() - other->GetWorldPosition();
		Vector3 velocity = ComputeCollisionVelocity(earthMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		hitTimer_ = kNoneHitTime_;
		break;
	}
}
