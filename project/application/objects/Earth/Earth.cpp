#include "Earth.h"

#include "framework/SUGER.h"

void Earth::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	earthHitTimer_ = 0;
	returnMoveTimer_ = 0;
	isAlive_ = true;

	inclinationRadian_ = DegreesToRadians(inclination_);
	SetRotate(Vector3(0.0f, 0.0f, -inclinationRadian_));
}

void Earth::Update() {

#ifdef _DEBUG
	ImGui::Begin("EarthVelo");
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.0f);
	ImGui::End();
#endif // _DEBUG


	if (earthHitTimer_ > 0) {
		earthHitTimer_--;
	}
	if (returnMoveTimer_ > 0) {
		returnMoveTimer_ -= SUGER::kDeltaTime_;
		if (returnMoveTimer_ < 0.1f) {
			returnMoveTimer_ = 0;
		}
	}

	// 移動量を足す
	if (isAlive_) {
		SetRotateY(GetRotate().y + 0.01f);
		SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
		// コライダーに移動量をセット
		GetCollider()->SetVelocity(velocity_);

		ReturnPosition();
	}

	MoveLimit();

	UpdateLifeState();
}

void Earth::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	float earthMass{};
	float playerMass{};
	switch (category) {
	case ColliderCategory::Player:
		if (earthHitTimer_ > 0) {
			break;
		}
		earthMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
		Vector3 velocity = ComputeCollisionVelocity(earthMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		earthHitTimer_ = kNoneHitTime_;
		returnMoveTimer_ = kReturnMoveTime_;
		break;
	case ColliderCategory::Fragment:
		HP_ -= 1;
		break;
	case ColliderCategory::Meteorite:
		HP_ -= 10;
		break;
	}
}

void Earth::MoveLimit() {
	Vector3 translate_ = GetTranslate();
	translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
	translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);
	SetTranslate(translate_);
}

void Earth::UpdateLifeState() {
	if (HP_ <= 0) {
		isAlive_ = false;

	}
}

void Earth::ReturnPosition() {
	Vector3 translate_ = GetTranslate();
	if (returnMoveTimer_ == 0) {
		if (translate_ != Vector3{ 0,0,0 }) {
			Vector3 normal = Normalize(Vector3{ 0, 0, 0 } - translate_);
			velocity_ = normal * returnSpeed_;

		}
		if (Length(translate_ - Vector3(0.0f, 0.0f, 0.0f)) < 0.01f) {
			velocity_ = { 0.0f,0.0f,0.0f };
			translate_ = { 0.0f,0.0f,0.0f };
		}
	}
}
