#include "Meteorite.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"
#include "objects/player/Player.h"


#include "system/FragmentManager/FragmentManager.h"

void Meteorite::Initialize(const std::string& name) {
	EntityController::Initialize(name);


}

void Meteorite::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void Meteorite::SetPlayer(Player* player){
	assert(player);
	player_ = player;
}

void Meteorite::SetFragmentManager(FragmentManager* fragmentManager) {
	assert(fragmentManager);
	fragmentManager_ = fragmentManager;
}

bool Meteorite::GetIsAlive() const {
	return isAlive_;
}

void Meteorite::Update() {

	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Meteorite::Behavior::kRoot:
			RootInitialize();
			break;
		case Meteorite::Behavior::kDagame:
			DamageInitialize();
			break;
		case Meteorite::Behavior::kBreak:
			BreakInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
	case Meteorite::Behavior::kRoot:
		RootUpdate();
		break;
	case Meteorite::Behavior::kDagame:
		DamageUpdate();
		break;
	case Meteorite::Behavior::kBreak:
		BreakUpdate();
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
	case ColliderCategory::Fragment:

		behaviorRequest_ = Behavior::kDagame;


		break;
	}

}

void Meteorite::RootInitialize() {
	// 通常色
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
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
		fragmentManager_->AddFragment(popPos, earth_,player_);
		emitTime_ = 0;
	}

}

void Meteorite::DamageInitialize() {
	hp_ -= other->GetDamage();
	damageTimer_ = kDamageTime_;
	SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Meteorite::DamageUpdate() {
	if (hp_ <= 0) {
		behaviorRequest_ = Meteorite::Behavior::kBreak;
	}

	damageTimer_--;
	if (damageTimer_ == 0) {
		behaviorRequest_ = Meteorite::Behavior::kRoot;
	}
}

void Meteorite::BreakInitialize() {
	// 破壊時色
	SetColor(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
	breakTimer_ = kBreakTime_;
}

void Meteorite::BreakUpdate() {
	breakTimer_--;
	if (breakTimer_ == 0) {
		SetIsDelete(true);
		isAlive_ = false;
	}
}

void Meteorite::SetSpeed(float speed) {
	speed_ = speed;
}
