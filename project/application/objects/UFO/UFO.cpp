#include "UFO.h"

#include "framework/SUGER.h"

#include <numbers>
#include "random/Random.h"

#include "system/UFOBulletManager/UFOBulletManager.h"
#include "system/DamagePieceManager/DamagePieceManager.h"
#include "objects/player/Player.h"

UFO::UFO() {
}

UFO::~UFO() {}

void UFO::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	// ランダムな位相を設定 (0 〜 2π の範囲で)
	phase_ = Random::GenerateFloat(0.0f, std::numbers::pi_v<float> *2.0f);

	shotTimer_ = shotInterval_;
}

bool UFO::GetIsAlive() const {
	return isAlive_;
}

void UFO::Update() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case UFO::Behavior::kRoot:
			RootInitialize();
			break;
		case UFO::Behavior::kDamage:
			DamageInitialize();
			break;
		case UFO::Behavior::kBreak:
			BreakInitialize();
			break;
		}


		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
	case UFO::Behavior::kRoot:
		RootUpdate();
		break;
	case UFO::Behavior::kDamage:
		DamageUpdate();
		break;
	case UFO::Behavior::kBreak:
		BreakUpdate();
		break;
	}
}

void UFO::OnCollision(Collider* other) {

	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	switch (category) {
	case ColliderCategory::Moon:
		if (behavior_ == Behavior::kRoot) {
			behaviorRequest_ = Behavior::kDamage;
		}
		break;
	}

}

void UFO::RootInitialize() {
	// 通常色
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void UFO::RootUpdate() {
	if (shotTimer_ > 0) {
		shotTimer_--;
	}

	if (shotTimer_ == 0) {
		ufoBulletManager_->AddUFOBullet(GetTranslate());
		shotTimer_ = shotInterval_;
	}

	// 時間経過を取得
	time_ += SUGER::kDeltaTime_;

	// 上下の揺れの速度を算出（位相を考慮）
	float waveVelocity = amplitude_ * frequency_ * (std::numbers::pi_v<float> *2.0f) *
		std::cos(time_ * frequency_ * (std::numbers::pi_v<float> *2.0f) + phase_);

	// 速度を計算（初期速度に揺れの速度を加算）
	velocity_ = initVelocity_ + Vector3(0.0f, waveVelocity, 0.0f);

	// 位置を更新
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);

	// コライダーに速度を適用
	GetCollider()->SetVelocity(velocity_);

	MoveLimit();
}

void UFO::DamageInitialize() {
	hp_--;
	damageTimer_ = kDamageTime_;
}

void UFO::DamageUpdate() {
	damageTimer_--;
	SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	if (hp_ <= 0) {
		behaviorRequest_ = Behavior::kBreak;
	}
	if (damageTimer_ == 0) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void UFO::BreakInitialize() {
	breakTimer_ = kBreakTime_;
}

void UFO::BreakUpdate() {
	SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	breakTimer_--;
	if (breakTimer_ == 0) {
		player_->GetScoreData().ufoNum_++;
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void UFO::SetVelocity(const Vector3& velocity) {
	initVelocity_ = velocity;
	velocity_ = initVelocity_;
}

void UFO::SetUFOBulletManager(UFOBulletManager* ufoBulletManager) {
	ufoBulletManager_ = ufoBulletManager;
}

void UFO::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void UFO::SetDamagePieceManager(DamagePieceManager* damagePieceManager) {
	damagePieceManager_ = damagePieceManager;
}


void UFO::KillMe() {
	hp_ = 1;
	behaviorRequest_ = Behavior::kDamage;

void UFO::SetPlayer(Player* player)
{
	player_ = player;

}
