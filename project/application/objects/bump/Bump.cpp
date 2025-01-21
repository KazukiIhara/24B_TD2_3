#include "Bump.h"

#include "objects/player/Player.h"


#include "framework/SUGER.h"

void Bump::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	SetParent(player_->GetWorldTransformPtr());
	invincibilityTimer_ = maxInvincibilityTime_;
}

void Bump::Update() {
	if (invincibilityTimer_ > 0) {
		invincibilityTimer_ -= SUGER::kDeltaTime_;
	}

	UpdateLifeState();

	LevelScaling();

}

void Bump::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
		case ColliderCategory::None:
			break;
		case ColliderCategory::Fragment:
			if (invincibilityTimer_ <= 0) {
				isAlive_ = false;
			}
			break;
		case ColliderCategory::Meteorite:
			if (invincibilityTimer_ <= 0) {
				isAlive_ = false;
			}
			break;
	}
}

void Bump::Move()
{

}

void Bump::MoveLimit()
{
}

void Bump::LevelScaling()
{
	// 
	if (experiencePoints_ >= 300) {
		experiencePoints_ = 0;
		level_++;
	}
	
	
	
	// レベル上限
	if (level_ > MaxLevel) {
		level_ = MaxLevel;
	}

	// レベルによる各調整
	switch (level_)
	{
	case 1:
		damageMultiplier_ = 1.0f;

		bounceFactor_ = 1.0f;

		scale_ = 0.5f;
		break;
	case 2:
		damageMultiplier_ = 2.0f;

		bounceFactor_ = 1.5f;

		scale_ = 1.0f;
		break;
	case 3:
		damageMultiplier_ = 2.0f;

		bounceFactor_ = 2.0f;

		scale_ = 1.5f;
		break;
	default:
		break;
	}

	GetCollider()->SetSize(scale_);
	GetCollider()->SetBounceFactor(bounceFactor_);
	GetCollider()->SetDamageMultiplier(damageMultiplier_);
}

void Bump::UpdateLifeState()
{
	if (!isAlive_) {
		SetIsDelete(true);
	}
}
