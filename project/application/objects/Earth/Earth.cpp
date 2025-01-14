#include "Earth.h"

void Earth::Initialize(const std::string& name) {
	EntityController::Initialize(name);

}

void Earth::Update() {

}

void Earth::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
		case ColliderCategory::None:
			break;
	}
}
