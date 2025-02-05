#include "UFO.h"

#include "framework/SUGER.h"

#include <numbers>
#include "random/Random.h"

#include "system/UFOBulletManager/UFOBulletManager.h"
#include "system/DamagePieceManager/DamagePieceManager.h"
#include "objects/player/Player.h"

UFO::UFO() {
}

UFO::~UFO() {}

void UFO::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	// ランダムな位相を設定 (0 〜 2π の範囲で)
	phase_ = Random::GenerateFloat(0.0f, std::numbers::pi_v<float> *2.0f);

	shotTimer_ = shotInterval_;
}

bool UFO::GetIsAlive() const {
	return isAlive_;
}

void UFO::Update() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case UFO::Behavior::kRoot:
			RootInitialize();
			break;
		case UFO::Behavior::kDamage:
			DamageInitialize();
			break;
		case UFO::Behavior::kBreak:
			BreakInitialize();
			break;
		}


		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
	case UFO::Behavior::kRoot:
		RootUpdate();
		break;
	case UFO::Behavior::kDamage:
		DamageUpdate();
		break;
	case UFO::Behavior::kBreak:
		BreakUpdate();
		break;
	}
}

void UFO::OnCollision(Collider* other) {

	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	switch (category) {
	case ColliderCategory::Moon:
		if (behavior_ == Behavior::kRoot) {
			behaviorRequest_ = Behavior::kDamage;
		}
		break;
	}

}

void UFO::RootInitialize() {
	// 通常色
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void UFO::RootUpdate() {
	if (shotTimer_ > 0) {
		shotTimer_--;
	}

	if (shotTimer_ == 0) {
		ShotEmit();
		ShotEllipseEmit();
		ufoBulletManager_->AddUFOBullet(GetTranslate());
		shotTimer_ = shotInterval_;
	}

	// 時間経過を取得
	time_ += SUGER::kDeltaTime_;

	// 上下の揺れの速度を算出（位相を考慮）
	float waveVelocity = amplitude_ * frequency_ * (std::numbers::pi_v<float> *2.0f) *
		std::cos(time_ * frequency_ * (std::numbers::pi_v<float> *2.0f) + phase_);

	// 速度を計算（初期速度に揺れの速度を加算）
	velocity_ = initVelocity_ + Vector3(0.0f, waveVelocity, 0.0f);

	// 位置を更新
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);

	// コライダーに速度を適用
	GetCollider()->SetVelocity(velocity_);

	MoveLimit();
}

void UFO::DamageInitialize() {
	hp_--;
	damageTimer_ = kDamageTime_;
}

void UFO::DamageUpdate() {
	damageTimer_--;
	SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	if (hp_ <= 0) {
		behaviorRequest_ = Behavior::kBreak;
	}
	if (damageTimer_ == 0) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void UFO::BreakInitialize() {
	breakTimer_ = kBreakTime_;
}

void UFO::BreakUpdate() {
	SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	breakTimer_--;
	if (breakTimer_ == 0) {
		player_->GetScoreData().ufoNum_++;




		for (int i = 0; i < 4; i++) {
			damagePieceManager_->AddDamagePiece(GetCollider()->GetWorldPosition(), {}, {}, {}, { 0.9f,1.0f }, {}, {}, false, 2);
			damagePieceManager_->AddDamagePiece(GetCollider()->GetWorldPosition(), {}, {}, {}, { 0.5f,0.6f }, {}, {}, false, 3);
		}

		Vector3 min = Vector3(-2, -2, 0);
		Vector3 max = Vector3(2, 2, 0);

		Vector3 maxVelo = ElementWiseMax(min, max);
		Vector3 minVelo = ElementWiseMin(min, max);

		emitterExplosionFire_->Emit();
		emitterExplosionDust_->Emit();
		emitterExplosionFireYellow_->Emit();




		isAlive_ = false;
		SetIsDelete(true);		
	}
}

void UFO::SetVelocity(const Vector3& velocity) {
	initVelocity_ = velocity;
	velocity_ = initVelocity_;
}

void UFO::SetUFOBulletManager(UFOBulletManager* ufoBulletManager) {
	ufoBulletManager_ = ufoBulletManager;
}

void UFO::MoveLimit() {
	if (GetTranslate().x > stageWidth_ || GetTranslate().x < -stageWidth_
		|| GetTranslate().y > stageHeight_ || GetTranslate().y < -stageHeight_) {
		isAlive_ = false;
		SetIsDelete(true);
	}
}

void UFO::SetDamagePieceManager(DamagePieceManager* damagePieceManager) {
	damagePieceManager_ = damagePieceManager;
}


void UFO::KillMe() {
	hp_ = 1;
	behaviorRequest_ = Behavior::kDamage;
}

void UFO::SetPlayer(Player* player) {
	player_ = player;

}

void UFO::SetPraticle(int count)
{
	particleNumber_ = count;

	// 爆発
	emitterExplosionFire_ = std::make_unique<EmitterController>();
	emitterExplosionFireYellow_ = std::make_unique<EmitterController>();
	emitterExplosionDust_ = std::make_unique<EmitterController>();

	CreateEmit("explosionDustParticle", "ufoDustExplosion", 40, 1.8f, { 0.75f, 1.5f }, { 0.039f, 0.039f, 0.039f }, emitterExplosionFire_.get());
	CreateEmit("dustParticle", "ufoDustExplosionFire", 40, 2.0f, { 0.75f, 1.5f }, { 1,0,0 }, emitterExplosionDust_.get());
	CreateEmit("dustParticle", "ufoDustExplosionFireYellow", 40, 1.5f, { 0.75f, 1.5f }, { 1,1,0 }, emitterExplosionFireYellow_.get());
	
	// 発射硝煙
	emitterShotDust_ = std::make_unique<EmitterController>();

	CreateEmit("ShotDustParticle", "shotDustParticle", 5, 1.2f, { 0.75f, 1.5f }, { 0.039f, 0.039f, 0.039f }, emitterShotDust_.get());


}

void UFO::CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit)
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

void UFO::EmitDust(const Vector3& pos, const Vector3& veloctiy)
{
	EmitMinMax(pos, Normalize(veloctiy) * 3, emitterExplosionFire_.get()); // 赤
	EmitMinMax(pos, Normalize(veloctiy) * 2, emitterExplosionFireYellow_.get()); //黄色
	EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterExplosionDust_.get());
	//EmitMinMax(pos * 1.5f, Normalize(veloctiy) * 2.5f, emitterDustBlack_.get());
}

void UFO::EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit)
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

void UFO::ShotEllipseEmit() {
	int numShots = 16;  // 発射する弾の数
	float baseSpeed = 2.5f;  // 基本速度

	float a = 3.0f;  // 長径の半径
	float b = 1.5f;  // 短径の半径

	// UFO からプレイヤーへの法線ベクトルを取得
	Vector3 normal = player_->GetCollider()->GetWorldPosition() - GetCollider()->GetWorldPosition();
	normal = Normalize(normal); // 法線ベクトルを正規化

	// 法線に直交するベクトルを求める
	Vector3 worldUp(0, 1, 0);
	Vector3 right = Cross(worldUp, normal); // 法線に直交するベクトル
	if (LengthSquared(right) < 0.001f) {
		// 万が一 right ベクトルがゼロになった場合は、別の軸を基準にする
		right = Vector3(1, 0, 0);
	}
	right = Normalize(right);

	Vector3 up = Cross(normal, right); // `up` は `right` と `normal` の外積
	up = Normalize(up);

	// 楕円状にパーティクルを発射
	emitterShotDust_->SetMaxPosition({0,0,0});
	emitterShotDust_->SetMinPosition({0,0,0});
	for (int i = 0; i < numShots; ++i) {
		float theta = i * (2.0f * 3.14159f / numShots); // 各弾の角度

		// 楕円状のオフセット
		Vector3 velocity = (right * (a * cosf(theta))) + (up * (b * sinf(theta)));

		// 基本速度を適用
		velocity = Normalize(velocity) * baseSpeed;

		// 法線方向のオフセットを適用（流れを持たせる）
		velocity += normal * baseSpeed;

		// パーティクルを発射
		emitterShotDust_->SetMaxSize(1.2f);
		emitterShotDust_->SetMinSize(1.2f);

		emitterShotDust_->SetMaxVelocity(velocity);
		emitterShotDust_->SetMinVelocity(velocity);
		emitterShotDust_->Emit();
	}
}

void UFO::ShotEmit()
{
	// UFO からプレイヤーへの法線ベクトルを取得
	Vector3 normal = player_->GetCollider()->GetWorldPosition() - GetCollider()->GetWorldPosition();
	normal = Normalize(normal); // 法線ベクトルを正規化

	float size = 1.0f;
	int numShots = 1;  // 発射する弾の数
	for (int i = 0; i < numShots; ++i) {
		Vector3 pos = normal * numShots;

		emitterShotDust_->SetMaxSize(size);
		emitterShotDust_->SetMinSize(size);
		emitterShotDust_->SetMaxVelocity(-normal);
		emitterShotDust_->SetMinVelocity(-normal);
		emitterShotDust_->Emit();
	}
}





