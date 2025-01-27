#include "Bump.h"

#include "objects/player/Player.h"

#include "framework/SUGER.h"

void Bump::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	//SetParent(player_->GetWorldTransformPtr());
	invincibilityTimer_ = maxInvincibilityTime_;

	isScaleUp = true;
	previousScale_ = scalelevel0_;
	targetScale_ = scalelevel1_;


}

void Bump::Update() {
	if (invincibilityTimer_ > 0) {
		invincibilityTimer_ -= SUGER::kDeltaTime_;
	}

	UpdateLifeState();

	LevelScaling();

}

void Bump::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
	case ColliderCategory::None:
		break;
	case ColliderCategory::Fragment:
		if (invincibilityTimer_ <= 0) {
			isAlive_ = false;
		}
		break;
	case ColliderCategory::Meteorite:
		if (invincibilityTimer_ <= 0) {
			isAlive_ = false;
		}
		break;
	}
}


void Bump::LevelScaling()
{
	// 
	if (experiencePoints_ >= 300) {
		experiencePoints_ = 0;
		if (level_ <= MaxLevel) {
			isScaleUp = true;
			if (level_ == MaxLevel) {
				isScaleUp = false;
			}
		}
		level_++;
	}



	// レベル上限
	if (level_ > MaxLevel) {
		level_ = MaxLevel;
	}



	// レベルによる各調整
	switch (level_)
	{
	case 1:
		damageMultiplier_ = 1.5f;

		bounceFactor_ = 1.0f;

		scale_ = scalelevel1_;

		break;
	case 2:
		damageMultiplier_ = 2.0f;

		bounceFactor_ = 1.5f;

		scale_ = scalelevel2_;

		previousScale_ = scalelevel1_;
		targetScale_ = scalelevel2_;
		break;
	case 3:
		damageMultiplier_ = 3.0f;

		bounceFactor_ = 2.0f;

		scale_ = scalelevel3_;

		previousScale_ = scalelevel2_;
		targetScale_ = scalelevel3_;
		break;
	default:
		break;
	}

	ScaleAmimetion();


	GetCollider()->SetBounceFactor(bounceFactor_);
	GetCollider()->SetDamageMultiplier(damageMultiplier_);
}

void Bump::UpdateLifeState()
{
	if (!isAlive_) {
		emitter_->Emit();
		SetIsDelete(true);
	}
}

void Bump::SetParticle(int count)
{
	emitter_ = std::make_unique<EmitterController>();
	std::string name_ = "bumpEmit" + std::to_string(count);
	// エミッターの作成
	SUGER::CreateEmitter(name_);
	emitter_->Initialize(name_);
	emitter_->SetParent(GetCollider()->GetWorldTransformPtr());

	// エミッターにパーティクルをセット
	emitter_->SetParticle("bumpParticle");
	// エミッターの発生個数を変更
	emitter_->SetCount(2);
	// エミッターの発生タイプを設定
	emitter_->SetEmitType(kRandom);
	// 繰り返し発生オフ
	emitter_->SetIsRepeat(false);
	// 
	emitter_->SetFrequency(0.01f);

	// 速度
	//emitter_->SetMaxVelocity(Vector3(2.0f, 2.0f, 2.0f));
	//emitter_->SetMinVelocity(Vector3(-2.0f, -2.0f, -2.0f));

	// サイズ
	emitter_->SetMaxSize(1.5f);
	emitter_->SetMinSize(1.5f);


	// カラー
	emitter_->SetMaxColor({ 1,0,0 });
	emitter_->SetMinColor({ 1,0,0 });
}

void Bump::ScaleAmimetion()
{
	if (isScaleUp) {
		animetionTimer_ += SUGER::kDeltaTime_;


		float t = static_cast<float>(animetionTimer_) / kAnimetionTimer_;
		if (t >= 1.0f) {
			t = 1.00f;
			isScaleUp = false;
			animetionTimer_ = 0.0f;
		}



		float newScale = Lerp(previousScale_, targetScale_, easeInBounce(t));
		GetCollider()->SetSize(newScale);
		SetScale((newScale));
	}
}


float Bump::easeInBounce(float t)
{
	return 1 - easeOutBounce(1 - t);
}

float Bump::easeOutBounce(float t)
{
	if (t < (1.0f / 2.75f)) {
		return 7.5625f * t * t;
	}
	else if (t < (2.0f / 2.75f)) {
		t -= (1.5f / 2.75f);
		return 7.5625f * t * t + 0.75f;
	}
	else if (t < (2.5f / 2.75f)) {
		t -= (2.25f / 2.75f);
		return 7.5625f * t * t + 0.9375f;
	}
	else {
		t -= (2.625f / 2.75f);
		return 7.5625f * t * t + 0.984375f;
	}
}
