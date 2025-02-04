#include "Moon.h"

#include "framework/SUGER.h"

#include "system/FragmentManager/FragmentManager.h"
#include "system/DamagePieceManager/DamagePieceManager.h"

#include "random/Random.h"

#include "objects/player/Player.h"

void Moon::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	returnMoveTimer_ = 0;


	SetTranslate(Vector3(5.0f, 0.0f, 0.0f));


}

void Moon::SetPlayer(Player* player) {
	assert(player);
	player_ = player;
}


void Moon::Update() {

#ifdef _DEBUG
	ImGui::Begin("EarthVelo");
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.0f);
	ImGui::End();
#endif // _DEBUG

	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Moon::Behavior::kRoot:
			RootInitialize();
			break;
		case Moon::Behavior::kAttack:
			AttackInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
	case Moon::Behavior::kRoot:
		RootUpdate();
		break;
	case Moon::Behavior::kAttack:
		AttackUpdate();
		break;
	}

	MoveLimit();

}

void Moon::UpdateTitle() {

#ifdef _DEBUG
	ImGui::Begin("EarthVelo");
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.0f);
	ImGui::End();
#endif // _DEBUG


	if (returnMoveTimer_ > 0) {
		returnMoveTimer_ -= SUGER::kDeltaTime_;
		if (returnMoveTimer_ < 0.1f) {
			returnMoveTimer_ = 0;
		}
	}

	// 移動量を足す
	SetRotateY(GetRotate().y + std::numbers::pi_v<float>*2.0f / aroundFrame_);

	MoveLimit();


}

void Moon::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	float moonMass{};
	float playerMass{};
	float fragmentMass{};
	HitParticleTimer_ -= SUGER::kDeltaTime_;
	switch (category) {
	case ColliderCategory::Player:
	{
		moonMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
		Vector3 velocity = ComputeCollisionVelocity(moonMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		returnMoveTimer_ = kReturnMoveTime_;

		for (int i = 0; i < 5; i++) {
			EmitDamegePiece2(-other->GetWorldPosition() - normal, velocity_, damagePieceManager_);
		}
	}
	break;
	case ColliderCategory::Fragment:

		if (behavior_ == Behavior::kCharge) {
			break;
		}


		moonMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		fragmentMass = other->GetMass();
		Vector3 fragmentVelocity = other->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
		Vector3 velocity = ComputeCollisionVelocity(moonMass, earthVelocity, fragmentMass, fragmentVelocity, 1.0f, normal);


		EmitDust(normal, normal);

		break;
	case ColliderCategory::Meteorite:
	{
		if (behavior_ == Behavior::kCharge) {
			break;
		}

		// 位置ベクトルを取得
		Vector3 posA = GetCollider()->GetWorldPosition();
		Vector3 posB = other->GetWorldPosition();
		// 各オブジェクトの「半径」相当の値を取得 (球体などの場合)
		float radiusA = GetCollider()->GetSize();
		float radiusB = other->GetSize();
		// 合計半径
		float sumRadius = radiusA + radiusB;
		// ２つのオブジェクト間の距離
		Vector3 diff = posA - posB;
		float distance = Length(diff);

		Vector3 normal = Normalize(posA - posB);
		if (distance < sumRadius) {
			SetTranslate(other->GetWorldPosition() + normal * (sumRadius + 0.1f));
		}


		moonMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 velocity = ComputeCollisionVelocity(moonMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		returnMoveTimer_ = kReturnMoveTime_;

		if (HitParticleTimer_ <= 0) {
			EmitDust(normal, normal);
			HitParticleTimer_ = 1;
		}


	}
	break;

	case ColliderCategory::UFO:
	{
		if (behavior_ == Behavior::kCharge) {
			break;
		}

		// 位置ベクトルを取得
		Vector3 posA = GetCollider()->GetWorldPosition();
		Vector3 posB = other->GetWorldPosition();
		// 各オブジェクトの「半径」相当の値を取得 (球体などの場合)
		float radiusA = GetCollider()->GetSize();
		float radiusB = other->GetSize();
		// 合計半径
		float sumRadius = radiusA + radiusB;
		// ２つのオブジェクト間の距離
		Vector3 diff = posA - posB;
		float distance = Length(diff);

		Vector3 normal = Normalize(posA - posB);
		if (distance < sumRadius) {
			SetTranslate(other->GetWorldPosition() + normal * (sumRadius + 0.1f));
		}


		moonMass = GetCollider()->GetMass();
		Vector3 moonVelocity = GetCollider()->GetVelocity();
		playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 velocity = ComputeCollisionVelocity(moonMass, moonVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		returnMoveTimer_ = kReturnMoveTime_;

		if (HitParticleTimer_ <= 0) {
			EmitDust(normal, normal);
			HitParticleTimer_ = 1;
		}

	}
	break;
	}
}

void Moon::MoveLimit() {
	Vector3 translate_ = GetTranslate();
	if (translate_.x > stageWidth_ || translate_.x < -stageWidth_) {
		translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
		velocity_.x = -velocity_.x;
	}
	if (translate_.y < -stageHeight_ || translate_.y > stageHeight_) {
		translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);
		velocity_.y = -velocity_.y;
	}
	SetTranslate(translate_);
}

void Moon::SetPraticle() {
	emitterDustRed_ = std::make_unique<EmitterController>();
	emitterDustYellow_ = std::make_unique<EmitterController>();
	emitterDustGray_ = std::make_unique<EmitterController>();
	emitterDustBlack_ = std::make_unique<EmitterController>();
	CreateEmit("earthDustParticle", "earthDustFire", 10, 1.0f, { 0.75f, 1.5f }, { 1, 0, 0 }, emitterDustRed_.get());
	CreateEmit("earthDustParticle", "earthDustFire2", 10, 0.7f, { 1.0f, 1.5f }, { 1, 1, 0 }, emitterDustYellow_.get());
	CreateEmit("earthDustParticle", "earthDustFire3", 55, 0.7f, { 1.5f, 2.5f }, { 0.412f, 0.412f, 0.412f }, emitterDustGray_.get());
	CreateEmit("earthDustParticle", "earthDustFire4", 55, 0.7f, { 1.5f, 2.5f }, { 0.039f, 0.039f, 0.039f }, emitterDustBlack_.get());


}

float Moon::GetAroundFrame() const {
	return aroundFrame_;
}

void Moon::RootRequest() {
	behaviorRequest_ = Behavior::kRoot;
	SetParent(player_->GetLocalTransform());
}

void Moon::ChargeRequest() {
	behaviorRequest_ = Behavior::kCharge;
}

void Moon::AttackRequest() {
	behaviorRequest_ = Behavior::kAttack;
}

void Moon::RootInitialize() {

}

void Moon::RootUpdate() {

}

void Moon::ChargeInitialize() {

}

void Moon::ChargeUpdate() {

}

void Moon::AttackInitialize() {
	Vector3 playerWorldPosition = ExtractionWorldPos(player_->GetWorldTransformPtr()->worldMatrix_);
	Vector3 worldPosition = ExtractionWorldPos(GetWorldTransformPtr()->worldMatrix_);
	Vector3 direction = Normalize(worldPosition - playerWorldPosition);
	SetTranslate(worldPosition);
	velocity_ = direction * speed_;
	GetWorldTransformPtr()->parent_ = nullptr;
}

void Moon::AttackUpdate() {
	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// コライダーに移動量をセット
	GetCollider()->SetVelocity(velocity_);
}

void Moon::BackUpdate() {
	Vector3 target = ExtractionWorldPos(player_->GetWorldTransformPtr()->worldMatrix_);
	Vector3 worldPos = ExtractionWorldPos(GetWorldTransformPtr()->worldMatrix_);
	velocity_ = Lerp(worldPos, target, 0.8f) - worldPos;

	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// コライダーに移動量をセット
	GetCollider()->SetVelocity(velocity_);
}

void Moon::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

void Moon::CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit) {
	std::string name_ = emitName;
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

void Moon::EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit) {
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

void Moon::EmitDamegePiece(const Vector3& pos, const Vector3& veloctiy, DamagePieceManager* damagePieceManager) {
	Vector3 velocity = (veloctiy);



	Vector3 radomVelo{};
	radomVelo.x = velocity.x * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.y = velocity.y * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.z = 0;

	Vector3 pospos = (pos);

	damagePieceManager->AddDamagePiece(-pospos, -Normalize(radomVelo), 0.8f, false, { 0.12f,0.17f }, { 0.604f, 0.384f, 0.161f ,1.0f }, { 1.5f,2.5f });
}

void Moon::EmitDamegePiece2(const Vector3& pos, const Vector3& veloctiy, DamagePieceManager* damagePieceManager) {
	Vector3 velocity = (veloctiy);



	Vector3 radomVelo{};
	radomVelo.x = velocity.x * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.y = velocity.y * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.z = 0;

	Vector3 pospos = (pos);

	damagePieceManager->AddDamagePiece(-pospos, -Normalize(radomVelo), 0.8f, false, { 0.05f,0.10f }, {}, { 0.5f,1.0f });
}

void Moon::EmitDust(const Vector3& pos, const Vector3& veloctiy) {
	EmitMinMax(pos, Normalize(veloctiy) * 3, emitterDustRed_.get()); // 赤
	EmitMinMax(pos, Normalize(veloctiy) * 2, emitterDustYellow_.get()); //黄色
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustGray_.get());
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustBlack_.get());

	for (int i = 0; i < 15; i++) {
		EmitDamegePiece(-GetCollider()->GetWorldPosition() + pos * 1.5f, Normalize(veloctiy) * 2.5f, damagePieceManager_);
	}
}

void Moon::ReturnPosition() {
	Vector3 translate_ = GetTranslate();
	if (returnMoveTimer_ == 0) {
		if (translate_ != Vector3{ 0,0,0 }) {
			Vector3 normal = Normalize(Vector3{ 0, 0, 0 } - translate_);
			velocity_ = normal * returnSpeed_;

		}
		if (Length(translate_ - Vector3(0.0f, 0.0f, 0.0f)) < 0.01f) {
			velocity_ = { 0.0f,0.0f,0.0f };
			translate_ = { 0.0f,0.0f,0.0f };
		}
	}
}
