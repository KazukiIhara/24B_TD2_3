#include "DamagePiece.h"

#include "framework/SUGER.h"

#include "random/Random.h"


void DamagePiece::Initialize(const std::string& name)
{
	EntityController::Initialize(name);

	// 方向
	float randomX = Random::GenerateFloat(-2, 2);
	float randomY = Random::GenerateFloat(-2, 2);

	velocity_ = { randomX ,randomY ,0 };

	// 生存時間
	float randomBreakTimer = Random::GenerateFloat(kMinBreakTime_, kMaxBreakTime_);

	breakTimer_ = randomBreakTimer;
	kBreakTime_ = randomBreakTimer;
	isAlive_ = true;

	float randomScale = Random::GenerateFloat(scaleMin, scaleMax);

	// 大きさ
	SetScale(randomScale);

	scale_ = randomScale;
}

void DamagePiece::Update()
{
	Move();

	breakTimer_  -= SUGER::kDeltaTime_;
	if (breakTimer_ <= 0) {
		SetIsDelete(true);
		isAlive_ = false;
	}
	if (isReduction) {
		ScaleReduction();
	}
}

void DamagePiece::OnCollision(Collider* other)
{
}

void DamagePiece::Move()
{
	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// 現在の移動量をセット
	GetCollider()->SetVelocity(velocity_);
	// 回転量を足す
	SetRotate(GetRotate() + velocity_ * SUGER::kDeltaTime_);
}

void DamagePiece::ScaleReduction()
{
	

	float t = breakTimer_ / kBreakTime_;

	float scale = Lerp(0.0f, scale_, t);

	SetScale(scale);
}
