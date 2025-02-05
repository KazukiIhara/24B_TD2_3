#include "Boss.h"

#include "framework/SUGER.h"
#include "system/UFOBulletManager/UFOBulletManager.h"
#include "objects/player/Player.h"

Boss::Boss() {}

Boss::~Boss() {}

void Boss::Initialize(const std::string& name) {
	EntityController::Initialize(name);

	SetScale(5.0f);
	SetRotateY(-std::numbers::pi_v<float> / 2.0f);

	hpGage_ = std::make_unique<Object2DController>();
	hpGage_->Initialize(SUGER::Create2DObject("0_hp", "white.png"));
	hpGage_->SetColor(Vector4(0.0f, 1.0f, 0.0f, 0.85f));
	hpGage_->SetPosition(Vector2(460.0f, 32.0f));
	hpGage_->SetSize(Vector2(1000.0f, 25.0f));
	hpGage_->SetIsActive(false);
}

void Boss::Update() {

#ifdef _DEBUG
	ImGui::Begin("Boss");
	ImGui::DragFloat("amplitude", &amplitude_, 0.01f);
	ImGui::DragFloat("frequency", &frequency_, 0.01f);
	ImGui::End();
#endif // _DEBUG

	if (behavior_ == Behavior::kRoot || behavior_ == Behavior::kDamage) {
		hpGage_->SetIsActive(true);
	} else {
		hpGage_->SetIsActive(false);
	}

	hpGage_->SetSize(Vector2(hp_ * 10.0f, 25.0f));

	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
			case Boss::Behavior::kIn:
				InInitialize();
				break;
			case Boss::Behavior::kRoot:
				RootInitialize();
				break;
			case Boss::Behavior::kDamage:
				DamageInitialize();
				break;
			case Boss::Behavior::kBreak:
				BreakInitialize();
				break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
		case Boss::Behavior::kIn:
			InUpdate();
			break;
		case Boss::Behavior::kRoot:
			RootUpdate();
			break;
		case Boss::Behavior::kDamage:
			DamageUpdate();
			break;
		case Boss::Behavior::kBreak:
			BreakUpdate();
			break;
	}


	// Bossの現在位置
	Vector3 bossPos = GetTranslate();

	// Boss → プレイヤー への方向ベクトルを計算
	Vector3 direction = Normalize(bossPos - player_->GetTranslate());

	// z軸回転角度を求める (ラジアン)
	float rotationZ = std::atan2(direction.y, direction.x);

	// Bossの回転を更新 (Z軸回転のみ適用)
	SetRotate(Vector3(0.0f, 0.0f, Lerp(GetRotate().z, rotationZ, 0.1f)));


}

void Boss::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	switch (category) {
		case ColliderCategory::None:
			break;
		case ColliderCategory::Player:
			break;
		case ColliderCategory::Moon:

			if (behavior_ == Behavior::kRoot) {
				behaviorRequest_ = Behavior::kDamage;
			}

			break;
		case ColliderCategory::Fragment:
			break;
		case ColliderCategory::Meteorite:
			break;
		case ColliderCategory::Bump:
			break;
		case ColliderCategory::DamagePiece:
			break;
		case ColliderCategory::UFO:
			break;
		case ColliderCategory::UFOBullet:
			break;
		case ColliderCategory::Boss:
			break;
		default:
			break;
	}

}

void Boss::RequestIn() {
	behaviorRequest_ = Behavior::kIn;
}

void Boss::RequestRoot() {
	behaviorRequest_ = Behavior::kRoot;
}

void Boss::InInitialize() {
	shotTimer_ = 0;
}

void Boss::InUpdate() {
	GetCollider()->SetVelocity(Vector3(-4.0f, 0.0f, 0.0f));
}

void Boss::RootInitialize() {

}

void Boss::RootUpdate() {
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	shotTimer_++;

	if (shotTimer_ == shotTime_) {
		// Bossの現在位置
		Vector3 bossPos = GetTranslate();

		// Boss → プレイヤー への方向ベクトルを計算
		Vector3 direction = Normalize(player_->GetTranslate() - bossPos);

		ufoBulletManager_->AddBossBullet(GetTranslate() + (direction * 6.0f) - Vector3(0.0f, 1.0f, 0.0f), 30.0f);
		shotTimer_ = 0;

		SUGER::PlayWaveSound("Boss_Attack.wav");
	}

	// 時間経過を取得
	time_ += SUGER::kDeltaTime_;

	// 上下の揺れの速度を算出（位相を考慮）
	float waveVelocity = amplitude_ * frequency_ * (std::numbers::pi_v<float> *2.0f) *
		std::cos(time_ * frequency_ * (std::numbers::pi_v<float> *2.0f));
	velocity_ = { 0.0f,waveVelocity,0.0f };
	SetTranslate(GetTranslate() + velocity_);

	GetCollider()->SetVelocity(velocity_);
}

void Boss::DamageInitialize() {
	hp_--;
	damageTimer_ = 0;
	SUGER::PlayWaveSound("MoonDamage.wav");
}

void Boss::DamageUpdate() {
	damageTimer_++;
	SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	if (hp_ == 0) {
		behaviorRequest_ = Behavior::kBreak;
	}
	if (damageTimer_ == damageTime_) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Boss::BreakInitialize() {
	breakTimer_ = 0;
}

void Boss::BreakUpdate() {
	breakTimer_++;
	SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	if (breakTimer_ == breakTime_) {
		isBossKill_ = true;
		SetIsActive(false);
	}
}

bool Boss::IsBossKill() const {
	return isBossKill_;
}

void Boss::AddColliderList() {
	if (behavior_ != Behavior::kNone && behavior_ != Behavior::kBreak) {
		SUGER::AddColliderList(this);
	}
}

void Boss::SetUFOBulletManager(UFOBulletManager* ufobulletManager) {
	ufoBulletManager_ = ufobulletManager;
}

void Boss::SetPlayer(Player* player) {
	player_ = player;
}
