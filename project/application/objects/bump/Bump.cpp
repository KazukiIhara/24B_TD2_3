#include "Bump.h"

void Bump::Initialize(const std::string& name) {

}

void Bump::Update() {



	LevelScaling();

}

void Bump::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
		case ColliderCategory::None:
			break;
		case ColliderCategory::Player:
			break;
		case ColliderCategory::Bump:
			break;
	}
}

void Bump::Move()
{

}

void Bump::MoveLimit()
{
}

void Bump::LevelScaling()
{
	// レベル上限
	if (level_ > MaxLevel) {
		level_ = MaxLevel;
	}

	// レベルによる各調整
	switch (level_)
	{
	case 1:
		damageMultiplier_ = 1.0f;

		bounceFactor_ = 1.0f;
		break;
	case 2:
		damageMultiplier_ = 2.0f;

		bounceFactor_ = 1.5f;

		break;
	case 3:
		damageMultiplier_ = 2.0f;

		bounceFactor_ = 2.0f;

		break;
	default:
		break;
	}


}
