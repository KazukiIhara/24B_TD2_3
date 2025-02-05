#include "UFOBulletManager.h"

#include "framework/SUGER.h"

#include "objects/player/Player.h"
#include "system/DamagePieceManager/DamagePieceManager.h"

void UFOBulletManager::Initialize(Player* player) {
	player_ = player;
}

void UFOBulletManager::Update() {
	ufoBullets_.remove_if([](const std::unique_ptr<UFOBullet>& fragment) {
		return !fragment->GetIsAlive();
		});

	// コンテナ内のかけらすべてを更新
	for (auto& ufoBullet : ufoBullets_) {
		ufoBullet->Update();
	}
}

void UFOBulletManager::AddUFOBullet(const Vector3& translate) {
	EulerTransform3D popTransform{};
	popTransform.translate = translate;

	Vector3 playerWorldPos = ExtractionWorldPos(player_->GetWorldTransformPtr()->worldMatrix_);
	Vector3 bulletPos = translate;

	Vector3 direction = Normalize(playerWorldPos - bulletPos);

	Vector3 velocity = direction * speed_;

	std::unique_ptr<UFOBullet> newBulet = std::make_unique<UFOBullet>();
	newBulet->Initialize(SUGER::CreateEntity("UFOBullet", "UFO_Bullet", popTransform));
	newBulet->CreateCollider(ColliderCategory::UFOBullet, kSphere, 0.8f);
	newBulet->SetVelocity(velocity);
	newBulet->SetScale(0.8f);
	newBulet->SetDamagePieceManager(damagePieceManager_);
	newBulet->SetSerialNumber(currentSerialNumber_);
	newBulet->SetPraticle(currentSerialNumber_);
	newBulet->UpdateWorldTransform();
	ufoBullets_.push_back(std::move(newBulet));

	currentSerialNumber_++;
}

void UFOBulletManager::AddBossBullet(const Vector3& translate, float spreadAngle) {
	EulerTransform3D popTransform{};
	popTransform.translate = translate;

	Vector3 playerWorldPos = ExtractionWorldPos(player_->GetWorldTransformPtr()->worldMatrix_);
	Vector3 bulletPos = translate;

	// プレイヤーへの基準方向
	Vector3 direction = Normalize(playerWorldPos - bulletPos);

	// 基準の弾を生成
	AddSingleBullet(direction, translate);

	// ±spreadAngle 度の方向に弾を追加
	Vector3 leftDirection = RotateVectorZ(direction, DegreesToRadians(spreadAngle));
	Vector3 rightDirection = RotateVectorZ(direction, DegreesToRadians(-spreadAngle));

	AddSingleBullet(leftDirection, translate);
	AddSingleBullet(rightDirection, translate);
}

void UFOBulletManager::AddSingleBullet(const Vector3& direction, const Vector3& translate) {
	Vector3 velocity = direction * speed_;

	std::unique_ptr<UFOBullet> newBullet = std::make_unique<UFOBullet>();
	newBullet->Initialize(SUGER::CreateEntity("UFOBullet", "UFO_Bullet", EulerTransform3D{ .translate = translate }));
	newBullet->CreateCollider(ColliderCategory::UFOBullet, kSphere, 0.8f);
	newBullet->SetVelocity(velocity);
	newBullet->SetScale(0.8f);
	newBullet->SetDamagePieceManager(damagePieceManager_);
	newBullet->SetSerialNumber(currentSerialNumber_);
	newBullet->SetPraticle(currentSerialNumber_);
	newBullet->UpdateWorldTransform();

	ufoBullets_.push_back(std::move(newBullet));
	currentSerialNumber_++;
}


void UFOBulletManager::AddColliderList() {
	for (auto& ufoBullet : ufoBullets_) {
		if (ufoBullet->GetIsAlive()) {
			SUGER::AddColliderList(ufoBullet.get());
		}
	}
}

void UFOBulletManager::SetPlayer(Player* player) {
	player_ = player;
}

void UFOBulletManager::SetDamagePieceManager(DamagePieceManager* damagePieceManager) {
	damagePieceManager_ = damagePieceManager;
}

void UFOBulletManager::KillAll() {
	for (auto& ufoBullet : ufoBullets_) {
		if (ufoBullet->GetIsAlive()) {
			ufoBullet->KillMe();
		}
	}
}
