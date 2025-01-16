#include "Skydome.h"

void Skydome::Initialize(const std::string& name) {
	EntityController::Initialize(name);

}

void Skydome::Update() {
	SetRotateY(GetRotate().y + speed_);
}

void Skydome::OnCollision(Collider* other) {
}
