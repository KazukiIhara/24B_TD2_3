#include "Bump.h"

void Bump::Initialize(const std::string& name) {

}

void Bump::Update() {

}

void Bump::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
		case ColliderCategory::None:
			break;
	}
}
