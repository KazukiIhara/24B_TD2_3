#include "UFOBulletManager.h"

#include "framework/SUGER.h"

void UFOBulletManager::Initialize() {

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

void UFOBulletManager::AddUFOBullet(const Vector3& translate, const Vector3& velocity) {
	EulerTransform3D popTransform{};
	popTransform.translate = translate;

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
