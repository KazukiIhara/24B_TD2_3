#include "Fragment.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"
#include "objects/player/Player.h"

#include "random/Random.h"

void Fragment::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	velocity_ = CalculateDirection(GetTranslate(), earth_->GetTranslate(), 60.0f);

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

	MoveLimit(); // 移動制限の処理

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

		GetCollider()->SetDamage(1.0f * other->GetDamageMultiplier());

		Vector3 velocity = ComputeCollisionVelocity(fragmentMass, fragmentVelocity, playerMass, playerVelocity, 10.0f * bounceFactor, normal);
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
	Move();
}

void Fragment::Move() {
	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// 現在の移動量をセット
	GetCollider()->SetVelocity(velocity_);
	// 回転量を足す
	SetRotate(GetRotate() + velocity_ * SUGER::kDeltaTime_);
}

void Fragment::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		HP_ = 0;
	}
}

void Fragment::UpdateLifeState() {
	if (HP_ <= 0) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void Fragment::SetSpeed(float speed) {
	speed_ = speed;
}

Vector3 Fragment::CalculateDirection(const Vector3& startPosition, const Vector3& targetPosition, float spreadAngle) {
	Vector3 direction = targetPosition - startPosition;
	direction = Normalize(direction);

	// ランダムな角度を生成 (-spreadAngle ～ +spreadAngle)
	float randomYaw = Random::GenerateFloat(-spreadAngle, spreadAngle);

	// 角度をラジアンに変換
	float yawRad = DirectX::XMConvertToRadians(randomYaw);

	// Z軸回転の適用
	Vector3 finalDirection = {
		direction.x * std::cos(yawRad) - direction.y * std::sin(yawRad), // X成分
		direction.x * std::sin(yawRad) + direction.y * std::cos(yawRad), // Y成分
		0.0f // Z成分 (2Dゲームでは0固定)
	};

	// ベクトルを正規化して長さを1にする
	finalDirection = Normalize(finalDirection);
	return finalDirection;
}