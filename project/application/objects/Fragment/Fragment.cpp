#include "Fragment.h"

#include "framework/SUGER.h"

#include "objects/Earth/Moon.h"
#include "objects/player/Player.h"

#include "random/Random.h"



void Fragment::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	velocity_ = CalculateDirection(GetTranslate(), moon_->GetTranslate(), 60.0f);

	playerHitTimer_ = 0;
	isAlive_ = true;


}

void Fragment::SetEarth(Moon* earth) {
	assert(earth);
	moon_ = earth;
}

void Fragment::Update() {

	HitTimerUpdate();

	BehaviorChange();
	BehaviorUpdate();

	MoveLimit(); // 移動制限の処理

	Atmosphere(); // 大気圏内処理

	BumpHitEffect(); // たんこぶに当たったときの処理
}

void Fragment::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
	case ColliderCategory::Player:
		HP_ -= 3;
		UpdateLifeState(); // 生死処理

		EmitFragment(velocity_);

		// スコア関係
		player_->GetScoreData().score_ += score_;
		player_->GetScoreData().fragmentNum_++;
		break;
	case ColliderCategory::Meteorite:

		HP_ -= 3;
		UpdateLifeState(); // 生死処理

		EmitFragment(velocity_);


		emitter_->SetMaxSize(1.8f);
		emitter_->SetMinSize(1.8f);
		emitter_->SetCount(10);
		emitter_->SetMaxVelocity({});
		emitter_->SetMinVelocity({});
		emitterDust_->SetMaxSize(2.0f);
		emitterDust_->SetMinSize(2.0f);
		emitterDust_->SetCount(10);
		emitterDust_->SetMaxVelocity({});
		emitterDust_->SetMinVelocity({});

		emitter_->Emit();
		emitterDust_->Emit();
		break;
	case ColliderCategory::Moon:
		if (playerHitTimer_ > 0) {
			break;
		}
		{
			float fragmentMass = GetCollider()->GetMass();
			Vector3 fragmentVelocity = GetCollider()->GetVelocity();
			float playerMass = other->GetMass();
			Vector3 playerVelocity = other->GetVelocity();
			Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
			Vector3 velocity = ComputeCollisionVelocity(fragmentMass, fragmentVelocity, playerMass, playerVelocity, 10.0f, normal);
			velocity_ = velocity;
			GetCollider()->SetDamage(1.0f);

			EmitFragment(velocity_);

			playerHitTimer_ = 0.5f;
		}
		HP_ -= 3;
		UpdateLifeState(); // 
		// スコア関係
		player_->GetScoreData().score_ += score_;
		player_->GetScoreData().fragmentNum_++;
		break;
	case ColliderCategory::Bump:
		float fragmentMass = GetCollider()->GetMass();
		Vector3 fragmentVelocity = GetCollider()->GetVelocity();
		float playerMass = player_->GetCollider()->GetMass();
		Vector3 playerVelocity = player_->GetCollider()->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - player_->GetCollider()->GetWorldPosition());

		float bounceFactor = other->GetBounceFactor();

		GetCollider()->SetDamage(1.0f * other->GetDamageMultiplier());

		Vector3 velocity = ComputeCollisionVelocity(fragmentMass, fragmentVelocity, playerMass, playerVelocity, 10.0f * bounceFactor, normal);
		velocity_ = Normalize(velocity);

		// 反射の比率を固定
		velocity_ = (velocity_ * 20) * bounceFactor;


		isBumpHit = true;
		break;
	}
}

void Fragment::HitTimerUpdate() {
	if (playerHitTimer_ > 0) {
		playerHitTimer_ -= SUGER::kDeltaTime_;
	}
}

void Fragment::BehaviorChange() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Fragment::Behavior::kRoot:
			RootInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}
}

void Fragment::BehaviorUpdate() {
	// ふるまい
	switch (behavior_) {
	case Fragment::Behavior::kRoot:
		RootUpdate();
		break;
	}
}

void Fragment::RootInitialize() {

}

void Fragment::RootUpdate() {
	Move();
}

void Fragment::Move() {
	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// 現在の移動量をセット
	GetCollider()->SetVelocity(velocity_);
	// 回転量を足す
	SetRotate(GetRotate() + velocity_ * SUGER::kDeltaTime_);
}

void Fragment::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void Fragment::UpdateLifeState() {
	if (HP_ <= 0) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void Fragment::Atmosphere() {
	float color = 1.0f; // 初期カラー値

	if (atmosphereRenge >= Length(GetCollider()->GetWorldPosition() - player_->GetCollider()->GetWorldPosition())) {
		// 対象が地球の大気圏内にいる場合、カラーを変化させます
		color = (Length(GetCollider()->GetWorldPosition() - player_->GetCollider()->GetWorldPosition()) / atmosphereRenge);

		Vector3 min = -(velocity_ * 0.85f);
		Vector3 max = -(velocity_ * 1.15f);

		Vector3 maxVelo = ElementWiseMax(min, max);
		Vector3 minVelo = ElementWiseMin(min, max);


		emitter_->SetMaxSize(0.8f);
		emitter_->SetMinSize(0.8f);
		emitter_->SetCount(1);
		emitter_->SetMaxVelocity(maxVelo);
		emitter_->SetMinVelocity(minVelo);
		emitterDust_->SetMaxSize(1.0f);
		emitterDust_->SetMinSize(1.0f);
		emitterDust_->SetCount(1);
		emitterDust_->SetMaxVelocity(maxVelo);
		emitterDust_->SetMinVelocity(minVelo);

		emitter_->Emit();
		emitterDust_->Emit();
	}

	color = (std::clamp)(color, 0.0f, 1.0f);
	// カラーを設定します（1.0 - 赤色, 0.0 - 黒色）
	SetColor({ 1, color, color, 1 });
}

void Fragment::SetSpeed(float speed) {
	speed_ = speed;
}

Vector3 Fragment::CalculateDirection(const Vector3& startPosition, const Vector3& targetPosition, float spreadAngle) {
	Vector3 direction = targetPosition - startPosition;
	direction = Normalize(direction);

	// ランダムな角度を生成 (-spreadAngle ～ +spreadAngle)
	float randomYaw = Random::GenerateFloat(-spreadAngle, spreadAngle);

	// 角度をラジアンに変換
	float yawRad = DirectX::XMConvertToRadians(randomYaw);

	// Z軸回転の適用
	Vector3 finalDirection = {
		direction.x * std::cos(yawRad) - direction.y * std::sin(yawRad), // X成分
		direction.x * std::sin(yawRad) + direction.y * std::cos(yawRad), // Y成分
		0.0f // Z成分 (2Dゲームでは0固定)
	};

	// ベクトルを正規化して長さを1にする
	finalDirection = Normalize(finalDirection);
	return finalDirection;
}

void Fragment::SetPraticle(int count) {
	particleNumber_ = count;

	emitter_ = std::make_unique<EmitterController>();
	emitterDust_ = std::make_unique<EmitterController>();
	emitterFragment_ = std::make_unique<EmitterController>();
	CreateEmit("dustParticle", "dustEmitter", 1, 0.8f, { 1,1,0 }, emitter_.get());
	CreateEmit("dustParticle", "dustFire", 1, 1.0f, { 1,0,0 }, emitterDust_.get());
	CreateEmit("fragmentParticle", "fragment", 5, 0.5f, { 1,1,1 }, emitterFragment_.get());

}

void Fragment::CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector3 color, EmitterController* emit) {
	std::string name_ = emitName + std::to_string(particleNumber_);
	// エミッターの作成
	SUGER::CreateEmitter(name_);
	emit->Initialize(name_);
	emit->SetParent(GetCollider()->GetWorldTransformPtr());

	// エミッターにパーティクルをセット
	emit->SetParticle(praticleName);
	// エミッターの発生個数を変更
	emit->SetCount(count);
	// エミッターの発生タイプを設定
	emit->SetEmitType(kRandom);
	// 繰り返し発生オフ
	emit->SetIsRepeat(false);
	// 
	emit->SetFrequency(0.01f);

	// 速度
	emit->SetMaxVelocity(Vector3(2.0f, 2.0f, 2.0f));
	emit->SetMinVelocity(Vector3(-2.0f, -2.0f, -2.0f));

	// サイズ
	emit->SetMaxSize(size);
	emit->SetMinSize(size);


	// カラー
	emit->SetMaxColor(color);
	emit->SetMinColor(color);
}

void Fragment::DustEmit() {

}

void Fragment::EmitFragment(const Vector3& velo) {
	Vector3 velocity = Normalize(velo);

	Vector3 min = -(velocity * 0.25f);
	Vector3 max = -(velocity * 2.5f);

	Vector3 maxVelo = ElementWiseMax(min, max);
	Vector3 minVelo = ElementWiseMin(min, max);

	emitterFragment_->SetMinPosition(minVelo);
	emitterFragment_->SetMaxPosition(maxVelo);

	emitterFragment_->SetMaxVelocity(maxVelo);
	emitterFragment_->SetMinVelocity(minVelo);
	emitterFragment_->Emit();
}

void Fragment::BumpHitEffect() {
	if (isBumpHit) {
		effectTimer += SUGER::kDeltaTime_;

		if (effectTimer < kEffectTimer) {
			Vector3 min = -(velocity_ * 0.85f);
			Vector3 max = -(velocity_ * 1.15f);

			Vector3 maxVelo = ElementWiseMax(min, max);
			Vector3 minVelo = ElementWiseMin(min, max);


			emitter_->SetMaxSize(0.8f);
			emitter_->SetMinSize(0.8f);
			emitter_->SetCount(1);
			emitter_->SetMaxVelocity(maxVelo);
			emitter_->SetMinVelocity(minVelo);
			emitterDust_->SetMaxSize(1.0f);
			emitterDust_->SetMinSize(1.0f);
			emitterDust_->SetCount(1);
			emitterDust_->SetMaxVelocity(maxVelo);
			emitterDust_->SetMinVelocity(minVelo);

			emitter_->Emit();
			emitterDust_->Emit();
		}
	}
}



