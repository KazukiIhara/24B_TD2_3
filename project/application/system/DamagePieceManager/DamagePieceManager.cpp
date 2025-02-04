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

void DamagePieceManager::AddDamagePiece(const Vector3& popTranslate, const Vector3& velo, const float& renge, const bool& isReduction, const Vector2& scaleMinMax, const Vector4& color, const Vector2& lifeMInMaxTime, const bool isRand, const int numPiece)
{
	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	popTransform.translate += Random::GenerateVector3(-renge, renge);
	popTransform.translate.z = 0;
	popTransform.scale;
	// 新しいかけらを作成
	std::unique_ptr<DamagePiece> newDamagePiece = std::make_unique<DamagePiece>();

	int rand = Random::GenerateInt32_t(0, 1);

	// スケール
	if (0 != Length(scaleMinMax)) {
		newDamagePiece->SetScaleMinMax(scaleMinMax);
	}

	// 生存時間
	if (0 != Length(lifeMInMaxTime)) {
		newDamagePiece->SetLifeMinMax(lifeMInMaxTime);
	}

	if (isRand) {
		if (rand == 0) {
			newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece", "DamagePiece", popTransform));
		}
		else {
			newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece2", "DamagePiece2", popTransform));
		}
	}
	else {
		switch (numPiece)
		{
		case 0:
			newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece", "DamagePiece", popTransform));
			break;
		case 1:
			newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece2", "DamagePiece2", popTransform));
			break;
		case 2:
			newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece3", "DamagePiece3", popTransform));

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		default:
			newDamagePiece->Initialize(SUGER::CreateEntity("DamagePiece", "DamagePiece", popTransform));
			break;
		}
	}


	if (0.0f != Length(velo)) {
		newDamagePiece->InitVelocity(velo);
	}
	if (isReduction) {
		newDamagePiece->SetIsReduction();
	}

	newDamagePiece->CreateCollider(ColliderCategory::DamagePiece, kSphere, 0.01f);

	newDamagePiece->GetCollider()->SetMass(1.0f);

	// 色
	if (0 != Length(color)) {
		newDamagePiece->SetColor(color);
	}



	newDamagePiece->UpdateWorldTransform();

	// 追加
	damagePiece_.push_back(std::move(newDamagePiece));
}
