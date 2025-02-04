#include "Meteorite.h"

#include "framework/SUGER.h"

#include "objects/Earth/Moon.h"
#include "objects/player/Player.h"


#include "system/FragmentManager/FragmentManager.h"
#include "system/DamagePieceManager/DamagePieceManager.h"

#include"manager/scene/SceneManager.h"


void Meteorite::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	SetScale(1.4f);
	damagePieceTime_ = kDamagePieceTime_;
}

void Meteorite::SetEarth(Moon* earth) {
	assert(earth);
	moon_ = earth;
}

void Meteorite::SetPlayer(Player* player) {
	assert(player);
	player_ = player;
}

void Meteorite::SetFragmentManager(FragmentManager* fragmentManager) {
	assert(fragmentManager);
	fragmentManager_ = fragmentManager;
}

void Meteorite::SetDamagePieceManager(DamagePieceManager* damagePieceManager) {
	assert(damagePieceManager);
	damagePieceManager_ = damagePieceManager;
}

bool Meteorite::GetIsAlive() const {
	return isAlive_;
}

void Meteorite::Update() {

	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Meteorite::Behavior::kRoot:
			RootInitialize();
			break;
		case Meteorite::Behavior::kDagame:
			DamageInitialize(damege_);
			break;
		case Meteorite::Behavior::kBreak:
			BreakInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
	case Meteorite::Behavior::kRoot:
		RootUpdate();
		break;
	case Meteorite::Behavior::kDagame:
		DamageUpdate();
		break;
	case Meteorite::Behavior::kBreak:
		BreakUpdate();
		break;
	}
	Atmosphere();
}

void Meteorite::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void Meteorite::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	float earthMass{};
	float playerMass{};
	float fragmentMass{};
	switch (category) {
	case ColliderCategory::None:

		break;
	case ColliderCategory::Fragment:

		if (behavior_ == Behavior::kRoot) {
			damege_ = other->GetDamage();

			if (damege_ == 0) {
				return;
			}

			behaviorRequest_ = Behavior::kDagame;

			earthMass = GetCollider()->GetMass();
			Vector3 earthVelocity = GetCollider()->GetVelocity();
			fragmentMass = other->GetMass();
			Vector3 fragmentVelocity = other->GetVelocity();
			Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());

			EmitDust(normal, normal);
		}
		break;
	case ColliderCategory::Moon:
		if (behavior_ == Behavior::kRoot) {
			behaviorRequest_ = Behavior::kDagame;
		}
		break;
	case ColliderCategory::Player:
		if (behavior_ == Behavior::kRoot) {
			behaviorRequest_ = Behavior::kDagame;
		}
		break;
	}

}

void Meteorite::RootInitialize() {
	// 通常色
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Meteorite::RootUpdate() {
	// 目標に対して保管移動
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	SetRotate(GetRotate() + Vector3(speed_, speed_, speed_ * SUGER::kDeltaTime_));

	MoveLimit();

	GetCollider()->SetVelocity(velocity_);
}

void Meteorite::DamageInitialize(float damage) {
	hp_--;
	damageTimer_ = kDamageTime_;
	SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	SwitchingHPModel();
}

void Meteorite::DamageUpdate() {
	if (hp_ <= 0) {
		behaviorRequest_ = Meteorite::Behavior::kBreak;
	}

	damageTimer_--;
	if (damageTimer_ == 0) {
		behaviorRequest_ = Meteorite::Behavior::kRoot;
	}
}

void Meteorite::BreakInitialize() {
	// 破壊時色
	SetColor(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
	breakTimer_ = kBreakTime_;
}

void Meteorite::BreakUpdate() {
	breakTimer_--;

	if (breakTimer_ == 0) {
		SetIsDelete(true);

		isAlive_ = false;
		for (int i = 0; i < 10; i++) {
			damagePieceManager_->AddDamagePiece(GetCollider()->GetWorldPosition());
		}

		// スコア関係
		player_->GetScoreData().score_ += score_;
		player_->GetScoreData().meteoriteNum_++;

		Vector3 min = Vector3(-2, -2, 0);
		Vector3 max = Vector3(2, 2, 0);

		Vector3 maxVelo = ElementWiseMax(min, max);
		Vector3 minVelo = ElementWiseMin(min, max);



		emitterExplosionFire_->SetMaxVelocity(maxVelo);
		emitterExplosionFire_->SetMinVelocity(minVelo);
		emitterExplosionFireYellow_->SetMaxVelocity(maxVelo);
		emitterExplosionFireYellow_->SetMinVelocity(minVelo);

		emitterExplosionDust_->SetMaxVelocity(maxVelo);
		emitterExplosionDust_->SetMinVelocity(minVelo);

		emitterExplosionFire_->Emit();
		emitterExplosionDust_->Emit();
		emitterExplosionFireYellow_->Emit();
	}
}

void Meteorite::Atmosphere() {
	float color = 1.0f; // 初期カラー値

	if (atmosphereRenge >= Length(GetCollider()->GetWorldPosition() - player_->GetCollider()->GetWorldPosition())) {
		damagePieceTime_ -= SUGER::kDeltaTime_;

		// 対象が地球の大気圏内にいる場合、カラーを変化させます
		color = (Length(GetCollider()->GetWorldPosition() - player_->GetCollider()->GetWorldPosition()) / atmosphereRenge);

		Vector3 min = -(velocity_ * 0.85f);
		Vector3 max = -(velocity_ * 1.15f);

		Vector3 maxVelo = ElementWiseMax(min, max);
		Vector3 minVelo = ElementWiseMin(min, max);



		emitter_->SetMaxVelocity(maxVelo);
		emitter_->SetMinVelocity(minVelo);
		emitterDust_->SetMaxVelocity(maxVelo);
		emitterDust_->SetMinVelocity(minVelo);

		emitter_->Emit();
		emitterDust_->Emit();

		// 欠片
		if (damagePieceTime_ <= 0) {
			damagePieceManager_->AddDamagePiece(GetCollider()->GetWorldPosition(), -velocity_ * 3, 4.0f, true);
			damagePieceTime_ = kDamagePieceTime_;
		}
	}

	color = (std::clamp)(color, 0.0f, 1.0f);
	// カラーを設定します（1.0 - 赤色, 0.0 - 黒色）
	SetColor({ 1, color, color, 1 });
}

void Meteorite::SetSpeed(float speed) {
	speed_ = speed;
}

void Meteorite::SetPraticle(int count) {
	particleNumber_ = count;

	emitter_ = std::make_unique<EmitterController>();
	emitterDust_ = std::make_unique<EmitterController>();
	emitterExplosionFire_ = std::make_unique<EmitterController>();
	emitterExplosionFireYellow_ = std::make_unique<EmitterController>();
	emitterExplosionDust_ = std::make_unique<EmitterController>();
	CreateEmit("dustParticle", "dustMeteorite", 1, 5.8f, { 0.75f, 1.5f }, { 1,1,0 }, emitter_.get());
	CreateEmit("dustParticle", "dustMeteoriteFire", 1, 6.0f, { 0.75f, 1.5f }, { 1,0,0 }, emitterDust_.get());
	CreateEmit("explosionDustParticle", "dustExplosion", 40, 5.8f, { 0.75f, 1.5f }, { 0.039f, 0.039f, 0.039f }, emitterExplosionFire_.get());
	CreateEmit("dustParticle", "dustExplosionFire", 40, 6.0f, { 0.75f, 1.5f }, { 1,0,0 }, emitterExplosionDust_.get());
	CreateEmit("dustParticle", "dustExplosionFireYellow", 40, 5.0f, { 0.75f, 1.5f }, { 1,1,0 }, emitterExplosionFireYellow_.get());


	emitterDustRed_ = std::make_unique<EmitterController>();
	emitterDustYellow_ = std::make_unique<EmitterController>();
	emitterDustGray_ = std::make_unique<EmitterController>();
	emitterDustBlack_ = std::make_unique<EmitterController>();
	CreateEmit("earthDustParticle", "MeteoriteDustFire", 10, 1.0f, { 0.75f, 1.5f }, { 1, 0, 0 }, emitterDustRed_.get());
	CreateEmit("earthDustParticle", "MeteoriteDustFire2", 10, 0.7f, { 1.0f, 1.5f }, { 1, 1, 0 }, emitterDustYellow_.get());
	CreateEmit("earthDustParticle", "MeteoriteDustFire3", 55, 0.7f, { 1.5f, 2.5f }, { 0.412f, 0.412f, 0.412f }, emitterDustGray_.get());
	CreateEmit("earthDustParticle", "MeteoriteDustFire4", 55, 0.7f, { 1.5f, 2.5f }, { 0.039f, 0.039f, 0.039f }, emitterDustBlack_.get());

}

void Meteorite::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

void Meteorite::SwitchingHPModel() {
	if (hp_ <= 0) {
		SetModel("Meteorite3");
	} else if (hp_ <= 1) {
		SetModel("Meteorite3");
		for (int i = 0; i < 12; i++) {
			damagePieceManager_->AddDamagePiece(GetCollider()->GetWorldPosition(), {}, {}, true, { 0.5,1.0f }, {}, { 0.5f,1.0f });
		}
	} else if (hp_ <= 2) {
		SetModel("Meteorite2");
		for (int i = 0; i < 10; i++) {
			damagePieceManager_->AddDamagePiece(GetCollider()->GetWorldPosition(), {}, {}, true, { 0.5,1.0f }, {}, { 0.5f,1.0f });
		}
	} else {

	}
}

void Meteorite::CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit) {
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

	// 生存時間
	emit->SetMinLifeTime(lifeTime.x);
	emit->SetMaxLifeTime(lifeTime.y);


	// カラー
	emit->SetMaxColor(color);
	emit->SetMinColor(color);
}

void Meteorite::EmitDust(const Vector3& pos, const Vector3& veloctiy) {
	EmitMinMax(pos, Normalize(veloctiy) * 3, emitterDustRed_.get()); // 赤
	EmitMinMax(pos, Normalize(veloctiy) * 2, emitterDustYellow_.get()); //黄色
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustGray_.get());
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustBlack_.get());
}

void Meteorite::EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit) {
	Vector3 velocity = (veloctiy);

	Vector3 min = (velocity * 0.25f);
	Vector3 max = (velocity * 2.5f);

	Vector3 maxVelo = ElementWiseMax(min, max);
	Vector3 minVelo = ElementWiseMin(min, max);

	Vector3 pospos = (pos);

	min = (pospos * 0.25f);
	max = (pospos * 2.5f);

	Vector3 maxPos = ElementWiseMax(-min, -max);
	Vector3 minPos = ElementWiseMin(-min, -max);

	emit->SetMinPosition(minPos);
	emit->SetMaxPosition(maxPos);

	emit->SetMaxVelocity(maxPos);
	emit->SetMinVelocity(minPos);
	emit->Emit();
}
