#include "UFOBullet.h"

#include "framework/SUGER.h"

UFOBullet::UFOBullet() {
}

UFOBullet::~UFOBullet() {
}

void UFOBullet::Initialize(const std::string& name) {
	EntityController::Initialize(name);

}

void UFOBullet::Update() {
	// 位置を更新
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// コライダーに速度を適用
	GetCollider()->SetVelocity(velocity_);

	MoveLimit();
}

void UFOBullet::OnCollision(Collider* other) {

}

void UFOBullet::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

bool UFOBullet::GetIsAlive() const {
	return isAlive_;
}

void UFOBullet::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}
