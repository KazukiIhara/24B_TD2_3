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
}

void UFOBullet::OnCollision(Collider* other) {

}

void UFOBullet::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

bool UFOBullet::GetIsAlive() const {
	return isAlive_;
}