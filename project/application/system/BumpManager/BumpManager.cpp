#include "BumpManager.h"

#include "framework/SUGER.h"

#include"application/objects/player/Player.h"
#include "application/objects/Earth/Earth.h"

void BumpManager::Initialize() 
{
	bumps_.clear();
}

void BumpManager::Update()
{
	// コンテナ内のかけらすべてを更新
	for (auto& bump : bumps_) {
		bump->Update();
		if (10 >= Distance(earth_->GetTranslate(), player_->GetTranslate())) {
			bump->GrowthBump();
		}
	}

	bumps_.remove_if([](const std::unique_ptr<Bump>& bump) {
		return !bump->GetAlive();
		});

}

void BumpManager::AddBump(const Vector3& popTranslate)
{
	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;
	
	// 新しいかけらを作成
	std::unique_ptr<Bump> newBump = std::make_unique<Bump>();
	newBump->SetPlayer(player_);
	newBump->Initialize(SUGER::CreateEntity("Bump", "Bump", popTransform));
	newBump->CreateCollider(ColliderCategory::Bump, kSphere, 0.2f);
	newBump->SetSerialNumber(currentSerialNumber_);
	newBump->SetParticle(currentSerialNumber_);
	newBump->SetScale(0.0f);
	newBump->SetParent(player_->GetWorldTransformPtr());
	newBump->UpdateWorldTransform();
	// 追加
	bumps_.push_back(std::move(newBump));

	// シリアルナンバーをインクリメント
	currentSerialNumber_++;

}

void BumpManager::AddColliderList()
{
	
	for (auto& bump : bumps_) {
		SUGER::AddColliderList(bump.get());
	}
}




