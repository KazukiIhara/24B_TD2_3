#include "Fragment.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"
#include "objects/player/Player.h"



void Fragment::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	playerHitTimer_ = 0;
	isAlive_ = true;
}

void Fragment::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void Fragment::Update() {

	
	
	HitTimerUpdate();

	BehaviorChange();
	BehaviorUpdate();

	MoveLimit(); // 移動制限と反射の処理

	UpdateLifeState(); // 生死処理
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
			GetCollider()->SetDamage(1.0f);
		}
		break;
	case ColliderCategory::Meteorite:

		HP_ -= 3;
		break;
	case ColliderCategory::Earth:
		HP_ -= 3;
		break;
	case ColliderCategory::Bump:
		float fragmentMass = GetCollider()->GetMass();
		Vector3 fragmentVelocity = GetCollider()->GetVelocity();
		float playerMass = player_->GetCollider()->GetMass();
		Vector3 playerVelocity = player_->GetCollider()->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - player_->GetCollider()->GetWorldPosition());

		float bounceFactor = other->GetBounceFactor();

		GetCollider()->SetDamage(1.0f*other->GetDamageMultiplier());

		Vector3 velocity = ComputeCollisionVelocity(fragmentMass, fragmentVelocity, playerMass, playerVelocity,10.0f * bounceFactor, normal);
		velocity_ = velocity;
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

void Fragment::MoveLimit()
{
	Vector3 translate_ = GetTranslate();
	translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
	translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);
	SetTranslate(translate_);

	if (translate_.x <= -stageWidth_ || translate_.x >= stageWidth_) {
		velocity_.x *= -1;
		HP_--;
	}
	if (translate_.y <= -stageHeight_ || translate_.y >= stageHeight_) {
		velocity_.y *= -1;
		HP_--;
	}

}

void Fragment::UpdateLifeState()
{
	if (HP_ <= 0) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void Fragment::SetSpeed(float speed) {
	speed_ = speed;
}
