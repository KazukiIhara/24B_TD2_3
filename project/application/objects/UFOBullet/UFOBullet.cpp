#include "UFOBullet.h"

#include "framework/SUGER.h"
#include "system/DamagePieceManager/DamagePieceManager.h"

UFOBullet::UFOBullet() {
}

UFOBullet::~UFOBullet() {
}

void UFOBullet::Initialize(const std::string& name) {
	EntityController::Initialize(name);

}

void UFOBullet::Update() {
	// 位置を更新
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	SetRotate(GetRotate() + velocity_ * SUGER::kDeltaTime_);
	// コライダーに速度を適用
	GetCollider()->SetVelocity(velocity_);

	MoveLimit();
}

void UFOBullet::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	switch (category) {
	case ColliderCategory::Player:

		EmitDie();
		isAlive_ = false;
		SetIsDelete(true);
		break;
	case ColliderCategory::Moon:

		EmitDie();
		isAlive_ = false;
		SetIsDelete(true);
		break;
	case ColliderCategory::Meteorite:

		EmitDie();
		isAlive_ = false;
		SetIsDelete(true);
		break;
	}

}

void UFOBullet::SetVelocity(const Vector3& velocity) {
	velocity_ = velocity;
}

bool UFOBullet::GetIsAlive() const {
	return isAlive_;
}

void UFOBullet::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void UFOBullet::SetDamagePieceManager(DamagePieceManager* damagePieceManager) {
	damagePieceManager_ = damagePieceManager;
}


void UFOBullet::SetPraticle(int count)
{
	particleNumber_ = count;

	emitterExplosionFire_ = std::make_unique<EmitterController>();
	emitterExplosionFireYellow_ = std::make_unique<EmitterController>();
	emitterExplosionDust_ = std::make_unique<EmitterController>();

	CreateEmit("explosionDustParticle", "ufoDustExplosion", 40,	1.2f, { 0.25f, 0.4f }, { 0.039f, 0.039f, 0.039f }, emitterExplosionDust_.get());
	CreateEmit("dustParticle", "ufoDustExplosionFire", 40, 1.0f, { 0.25f, 0.4f }, { 1,0,0 }, emitterExplosionFire_.get());
	CreateEmit("dustParticle", "ufoDustExplosionFireYellow", 40, 0.8f, { 0.25f, 0.4f }, { 1,1,0 }, emitterExplosionFireYellow_.get());

	emitterExplosionFireYellow_->SetMaxVelocity({ 0,0,0 });
	emitterExplosionFireYellow_->SetMinVelocity({ 0,0,0 });
	emitterExplosionDust_->SetMaxVelocity({ 0,0,0 });
	emitterExplosionDust_->SetMinVelocity({ 0,0,0 });
	emitterExplosionFire_->SetMaxVelocity({ 0,0,0 });
	emitterExplosionFire_->SetMinVelocity({ 0,0,0 });
}

void UFOBullet::CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit)
{
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
	emit->SetMaxVelocity(Vector3(0.0f, 0.0f, 0.0f));
	emit->SetMinVelocity(Vector3(-0.0f, -0.0f, -0.0f));

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

void UFOBullet::EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit)
{
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

void UFOBullet::EmitDie()
{
	emitterExplosionFire_->Emit();
	emitterExplosionDust_->Emit();
	emitterExplosionFireYellow_->Emit();
}


void UFOBullet::KillMe() {
	isAlive_ = false;
	SetIsDelete(true);
}

