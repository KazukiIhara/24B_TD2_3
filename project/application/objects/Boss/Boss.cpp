#include "Boss.h"

Boss::Boss() {}

Boss::~Boss() {}

void Boss::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	SetScale(5.0f);
	SetRotateY(-std::numbers::pi_v<float> / 2.0f);
}
