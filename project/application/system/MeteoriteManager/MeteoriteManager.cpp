#include "MeteoriteManager.h"

#include "framework/SUGER.h"

#include "system/FragmentManager/FragmentManager.h"
#include "objects/Earth/Earth.h"

#include "random/Random.h"

void MeteoriteManager::Initialize(Moon* earth, Player* player, FragmentManager* fragmentManager, DamagePieceManager* damagePieceManager) {
	assert(earth);
	assert(player);
	assert(fragmentManager);
	assert(damagePieceManager);
	moon_ = earth;
	player_ = player;
	fragmentManager_ = fragmentManager;
	damagePieceManager_ = damagePieceManager;

	// コンテナをクリア
	meteorites_.clear();

	// グローバルデータグループを作成
	SUGER::AddGrobalDataGroup(kParamaterString_);
	// グローバルデータを挿入
	SUGER::AddGrobalDataItem(kParamaterString_, "Speed", speed_);

	speed_ = SUGER::GetGrobalDataValueFloat(kParamaterString_, "Speed");

	float screenWidth = 40.0f;
	float screenHeight = 22.5f;
	popPosition_[static_cast<uint32_t>(MeteoritePopPlace::LeftTop)] = { -screenWidth,screenHeight,0.0f };
	popPosition_[static_cast<uint32_t>(MeteoritePopPlace::LeftBottom)] = { -screenWidth, -screenHeight, 0.0f };
	popPosition_[static_cast<uint32_t>(MeteoritePopPlace::RightBottom)] = { screenWidth, -screenHeight, 0.0f };
	popPosition_[static_cast<uint32_t>(MeteoritePopPlace::RightTop)] = { screenWidth, screenHeight, 0.0f };

	popTimer_ = popIntervalTime_;

}

void MeteoriteManager::Update() {
	// グローバルデータクラスからデータを取得
	speed_ = SUGER::GetGrobalDataValueFloat(kParamaterString_, "Speed");

	PopMateorites();

	// 削除フラグの立った隕石を削除
	meteorites_.remove_if([](const std::unique_ptr<Meteorite>& meteorite) {
		return !meteorite->GetIsAlive();
		});

	// コンテナ内の隕石をすべて更新
	for (auto& meteorite : meteorites_) {
		meteorite->Update();
		meteorite->SetSpeed(speed_);
	}
}

void MeteoriteManager::AddMeteorite(const Vector3& popTranslate) {
	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	// 新しい隕石を作成
	std::unique_ptr<Meteorite> newMeteorite = std::make_unique<Meteorite>();
	newMeteorite->Initialize(SUGER::CreateEntity("Meteorite", "Meteorite", popTransform));
	newMeteorite->SetEarth(moon_);
	newMeteorite->SetPlayer(player_);
	newMeteorite->SetFragmentManager(fragmentManager_);
	newMeteorite->SetDamagePieceManager(damagePieceManager_);
	newMeteorite->CreateCollider(ColliderCategory::Meteorite, kSphere, 3.5f);
	//newMeteorite->

	newMeteorite->GetCollider()->SetMass(20000.0f);
	newMeteorite->SetPraticle(currentSerialNumber_);
	newMeteorite->UpdateWorldTransform();

	// 追加
	meteorites_.push_back(std::move(newMeteorite));

	currentSerialNumber_++;
}

void MeteoriteManager::AddColliderList() {
	for (auto& meteorite : meteorites_) {
		if (meteorite->GetIsAlive()) {
			if (meteorite->GetIsAlive()) {
				SUGER::AddColliderList(meteorite.get());
			}
		}
	}
}

void MeteoriteManager::PopMateorites() {
	if (popTimer_ > 0) {
		popTimer_--;
	} else {
		for (int32_t i = 0; i < popNum_; i++) {
			Vector3 popPosition = {
				popPosition_[static_cast<uint32_t>(popPlace_)].x + Random::GenerateFloat(-10.0f,10.0f),
				popPosition_[static_cast<uint32_t>(popPlace_)].y + Random::GenerateFloat(-10.0f,10.0f),
				0.0f
			};
			AddMeteorite(popPosition);

			int popPosNum = Random::GenerateUint32_t(0, 3);
			popPlace_ = static_cast<MeteoritePopPlace>(popPosNum);
			popTimer_ = popIntervalTime_;
		}

	}
}
