#include "Earth.h"

#include "framework/SUGER.h"

void Earth::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	earthHitTimer_ = 0;
	returnMoveTimer_ = 0;
}

void Earth::Update() {
	if (earthHitTimer_ > 0) {
		earthHitTimer_--;
	}
	if (returnMoveTimer_ > 0) {
		returnMoveTimer_ -= SUGER::kDeltaTime_;
	}

	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// コライダーに移動量をセット
	GetCollider()->SetVelocity(velocity_);

	ReturnPosition();

	MoveLimit();
}

void Earth::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
	case ColliderCategory::Player:
		if (earthHitTimer_ > 0) {
			break;
		}
		float earthMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		float playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
		Vector3 velocity = ComputeCollisionVelocity(earthMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		earthHitTimer_ = kNoneHitTime_;
		returnMoveTimer_ = kReturnMoveTime_;
		break;
	}
}

void Earth::MoveLimit()
{
	Vector3 translate_ = GetTranslate();
	translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
	translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);
	SetTranslate(translate_);
}

void Earth::ReturnPosition()
{
	Vector3 translate_ = GetTranslate();
	if (returnMoveTimer_ <= 0) {
		if (translate_ != Vector3{ 0,0,0 }) {
			Vector3 normal = Normalize(Vector3{ 0, 0, 0 } - translate_);
			velocity_ = normal * returnSpeed_;
			
		}
	}
}
