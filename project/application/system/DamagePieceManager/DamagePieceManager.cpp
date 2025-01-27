#include "DamagePieceManager.h"

#include "framework/SUGER.h"

#include "random/Random.h"

void DamagePieceManager::Initialize()
{
	// コンテナをクリア
	damagePiece_.clear();

}

void DamagePieceManager::Update()
{
	// コンテナ内のかけらすべてを更新
	for (auto& fragment : damagePiece_) {
		fragment->Update();
	}

	damagePiece_.remove_if([](const std::unique_ptr<DamagePiece>& damagePiece) {
		return !damagePiece->GetAlive();
		});

}

void DamagePieceManager::AddDamagePiece(const Vector3& popTranslate, const Vector3& velo, const float& renge, const bool& isReduction)
{
	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	popTransform.translate += Random::GenerateVector3(-renge, renge);
	popTransform.translate.z = 0;
	// 新しいかけらを作成
	std::unique_ptr<DamagePiece> newDamagePiece = std::make_unique<DamagePiece>();

	int rand = Random::GenerateInt32_t(0, 1);

	if (rand == 0) {
		newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece", "DamagePiece", popTransform));
	}
	else {
		newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece2", "DamagePiece2", popTransform));
	}
	if (0.0f != Length(velo)) {
		newDamagePiece->InitVelocity(velo);
	}
	if (isReduction) {
		newDamagePiece->SetIsReduction();
	}

	newDamagePiece->CreateCollider(ColliderCategory::DamagePiece, kSphere, 0.01f);
	
	newDamagePiece->GetCollider()->SetMass(1.0f);
	newDamagePiece->UpdateWorldTransform();

	// 追加
	damagePiece_.push_back(std::move(newDamagePiece));
}
