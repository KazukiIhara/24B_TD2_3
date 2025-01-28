#include "Earth.h"

#include "framework/SUGER.h"

#include "system/FragmentManager/FragmentManager.h"
#include "system/DamagePieceManager/DamagePieceManager.h"

#include "random/Random.h"

void Earth::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	returnMoveTimer_ = 0;
	isAlive_ = true;

	inclinationRadian_ = DegreesToRadians(inclination_);
	SetRotate(Vector3(0.0f, 0.0f, -inclinationRadian_));


}

void Earth::Update() {

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
	if (isAlive_) {
		SetRotateY(GetRotate().y + std::numbers::pi_v<float>*2.0f / aroundFrame_);
		SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
		// コライダーに移動量をセット
		GetCollider()->SetVelocity(velocity_);

		ReturnPosition();
	}

	MoveLimit();

	UpdateLifeState();
}

void Earth::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	float earthMass{};
	float playerMass{};
	float fragmentMass{};
	switch (category) {
	case ColliderCategory::Player:
	{
		earthMass = GetCollider()->GetMass();
		Vector3 earthVelocity = GetCollider()->GetVelocity();
		playerMass = other->GetMass();
		Vector3 playerVelocity = other->GetVelocity();
		Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
		Vector3 velocity = ComputeCollisionVelocity(earthMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
		velocity_ = velocity;
		returnMoveTimer_ = kReturnMoveTime_;

		for (int i = 0; i < 5; i++) {
			EmitDamegePiece2(-other->GetWorldPosition() - normal, velocity_, damagePieceManager_);
		}
	}
		break;
		case ColliderCategory::Fragment:
			earthMass = GetCollider()->GetMass();
			Vector3 earthVelocity = GetCollider()->GetVelocity();
			fragmentMass = other->GetMass();
			Vector3 fragmentVelocity = other->GetVelocity();
			Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
			Vector3 velocity = ComputeCollisionVelocity(earthMass, earthVelocity, fragmentMass, fragmentVelocity, 1.0f, normal);


			EmitDust(normal, normal);

			HP_ -= 5;
			isObjectHit = true;
			objectHitLevel = 1;
			break;
		case ColliderCategory::Meteorite:
		{
			earthMass = GetCollider()->GetMass();
			Vector3 earthVelocity = GetCollider()->GetVelocity();
			playerMass = other->GetMass();
			Vector3 playerVelocity = other->GetVelocity();
			Vector3 normal = Normalize(GetCollider()->GetWorldPosition() - other->GetWorldPosition());
			Vector3 velocity = ComputeCollisionVelocity(earthMass, earthVelocity, playerMass, playerVelocity, 1.0f, normal);
			velocity_ = velocity;
			returnMoveTimer_ = kReturnMoveTime_;


			EmitDust(normal, normal);

			isObjectHit = true;
			objectHitLevel = 2;
		}
		HP_ -= 25;
		break;
	}
}

void Earth::MoveLimit() {
	Vector3 translate_ = GetTranslate();
	if (translate_.x > stageWidth_ || translate_.x < -stageWidth_ ||
		translate_.y < -stageHeight_ || translate_.y > stageHeight_) {
		translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
		translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);
		SetTranslate(translate_);
		velocity_ = { 0.0f,0.0f,0.0f };
	}
}

void Earth::UpdateLifeState() {
	if (HP_ <= 0.0f) {
		isAlive_ = false;
		HP_ = 0.0f;
	}
}

float& Earth::GetHp() {
	return HP_;
}

void Earth::SetPraticle() {
	emitterDustRed_ = std::make_unique<EmitterController>();
	emitterDustYellow_ = std::make_unique<EmitterController>();
	emitterDustGray_ = std::make_unique<EmitterController>();
	emitterDustBlack_ = std::make_unique<EmitterController>();
	CreateEmit("earthDustParticle", "earthDustFire", 10, 1.0f, { 0.75f, 1.5f }, { 1, 0, 0 }, emitterDustRed_.get());
	CreateEmit("earthDustParticle", "earthDustFire2", 10, 0.7f, { 1.0f, 1.5f }, { 1, 1, 0 }, emitterDustYellow_.get());
	CreateEmit("earthDustParticle", "earthDustFire3", 55, 0.7f, { 1.5f, 2.5f }, { 0.412f, 0.412f, 0.412f }, emitterDustGray_.get());
	CreateEmit("earthDustParticle", "earthDustFire4", 55, 0.7f, { 1.5f, 2.5f }, { 0.039f, 0.039f, 0.039f }, emitterDustBlack_.get());


}

float Earth::GetAroundFrame() const {
	return aroundFrame_;
}

void Earth::CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit) {
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

void Earth::EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit) {
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

void Earth::EmitDamegePiece(const Vector3& pos, const Vector3& veloctiy, DamagePieceManager* damagePieceManager)
{
	Vector3 velocity = (veloctiy);

	

	Vector3 radomVelo{};
	radomVelo.x = velocity.x * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.y = velocity.y * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.z = 0;

	Vector3 pospos = (pos);

	damagePieceManager->AddDamagePiece(-pospos, -Normalize(radomVelo), 0.8f,false,{0.12f,0.17f},{ 0.604f, 0.384f, 0.161f ,1.0f},{1.5f,2.5f});
}

void Earth::EmitDamegePiece2(const Vector3& pos, const Vector3& veloctiy, DamagePieceManager* damagePieceManager)
{
	Vector3 velocity = (veloctiy);



	Vector3 radomVelo{};
	radomVelo.x = velocity.x * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.y = velocity.y * Random::GenerateFloat(0.25f, 2.5f);
	radomVelo.z = 0;

	Vector3 pospos = (pos);

	damagePieceManager->AddDamagePiece(-pospos, -Normalize(radomVelo), 0.8f, false, { 0.05f,0.10f }, {}, { 0.5f,1.0f });
}

void Earth::EmitDust(const Vector3& pos, const Vector3& veloctiy) {
	EmitMinMax(pos, Normalize(veloctiy) * 3, emitterDustRed_.get()); // 赤
	EmitMinMax(pos, Normalize(veloctiy) * 2, emitterDustYellow_.get()); //黄色
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustGray_.get());
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustBlack_.get());

	for (int i = 0; i < 15; i++) {
		EmitDamegePiece(-GetCollider()->GetWorldPosition() + pos * 1.5f, Normalize(veloctiy) * 2.5f, damagePieceManager_);
	}
}

void Earth::ReturnPosition() {
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
