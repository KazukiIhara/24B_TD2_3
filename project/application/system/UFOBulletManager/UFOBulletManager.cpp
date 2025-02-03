#include "UFOBulletManager.h"

#include "framework/SUGER.h"

#include "objects/player/Player.h"

void UFOBulletManager::Initialize(Player* player) {
	player_ = player;
}

void UFOBulletManager::Update() {
	// コンテナ内のかけらすべてを更新
	for (auto& fragment : ufoBullets_) {
		fragment->Update();
	}

	ufoBullets_.remove_if([](const std::unique_ptr<UFOBullet>& fragment) {
		return !fragment->GetIsAlive();
		});
}

void UFOBulletManager::AddUFOBullet(const Vector3& translate) {
	EulerTransform3D popTransform{};
	popTransform.translate = translate;

	Vector3 playerWorldPos = ExtractionWorldPos(player_->GetWorldTransformPtr()->worldMatrix_);
	Vector3 bulletPos = translate;

	Vector3 direction = Normalize(bulletPos - playerWorldPos);

	Vector3 velocity = direction * speed_;

	std::unique_ptr<UFOBullet> newBulet = std::make_unique<UFOBullet>();
	newBulet->Initialize(SUGER::CreateEntity("UFOBullet", "Fragment", popTransform));
	newBulet->CreateCollider(ColliderCategory::UFOBullet, kSphere, 0.8f);
	newBulet->SetVelocity(velocity);
	newBulet->SetSerialNumber(currentSerialNumber_);
	newBulet->UpdateWorldTransform();
	ufoBullets_.push_back(std::move(newBulet));

	currentSerialNumber_++;
}

void UFOBulletManager::AddColliderList() {
	for (auto& ufoBullet : ufoBullets_) {
		SUGER::AddColliderList(ufoBullet.get());
	}
}

void UFOBulletManager::SetPlayer(Player* player) {
	player_ = player;
}
