#include "PronamaChan.h"

void PronamaChan::Update() {
	// もし衝突フラグがtrueなら色を赤に
	if (hitFlag_) {
		SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		hitFlag_ = false;
	} else {
		SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}	
}

void PronamaChan::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	switch (category) {
	case kNone:
		hitFlag_ = true;
		break;
	}
}
