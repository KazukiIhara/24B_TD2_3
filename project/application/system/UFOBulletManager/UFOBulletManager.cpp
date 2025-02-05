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
	newBulet->SetSerialNumber(currentSerialNumber_);
	newBulet->SetDamagePieceManager(damagePieceManager_);
	newBulet->SetPraticle(currentSerialNumber_);
	newBulet->UpdateWorldTransform();
	ufoBullets_.push_back(std::move(newBulet));

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

void UFOBulletManager::SetDamagePieceManager(DamagePieceManager* damagePieceManager)
{
	damagePieceManager_ = damagePieceManager;
}
