#include "MeteoriteManager.h"

#include "framework/SUGER.h"

#include "system/FragmentManager/FragmentManager.h"
#include "objects/Earth/Moon.h"

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
	popPosition_[static_cast<uint32_t>(MeteoritePopPlace::Left)] = { -screenWidth,0.0f,0.0f };
	popPosition_[static_cast<uint32_t>(MeteoritePopPlace::Right)] = { screenWidth, 0.0f, 0.0f };

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
		meteorite->SetIsPop(isPop_);
	}

#ifdef _DEBUG
	ImGui::Begin("MeteoriteManager");
	ImGui::Text("num: %d", meteorites_.size());
	ImGui::End();


#endif // _DEBUG
}

void MeteoriteManager::AddMeteorite(const Vector3& popTranslate, const Vector3& velocity) {
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
	newMeteorite->SetVelocity(velocity);

	newMeteorite->GetCollider()->SetMass(20000.0f);
	newMeteorite->UpdateWorldTransform();

	newMeteorite->SetPraticle(currentSerialNumber_);

	// 追加
	meteorites_.push_back(std::move(newMeteorite));

	currentSerialNumber_++;
}

void MeteoriteManager::AddColliderList() {
	for (auto& meteorite : meteorites_) {
		if (meteorite->GetIsAlive()) {
			SUGER::AddColliderList(meteorite.get());
		}
	}
}

void MeteoriteManager::PopMateorites() {
	if (popTimer_ > 0) {
		popTimer_--;
	} else {
		for (int32_t i = 0; i < popNum_; i++) {
			Vector3 popPosition = {
				popPosition_[static_cast<uint32_t>(popPlace_)].x,
				popPosition_[static_cast<uint32_t>(popPlace_)].y + Random::GenerateFloat(-10.0f,10.0f),
				0.0f
			};

			switch (popPlace_) {
				case MeteoritePopPlace::Left:
					AddMeteorite(popPosition, Vector3(0.8f, 0.0f, 0.0f));
					break;
				case MeteoritePopPlace::Right:
					AddMeteorite(popPosition, Vector3(-0.8f, 0.0f, 0.0f));
					break;
			}


			int popPosNum = Random::GenerateUint32_t(0, 1);
			popPlace_ = static_cast<MeteoritePopPlace>(popPosNum);
			popTimer_ = popIntervalTime_;
		}

	}
}

void MeteoriteManager::SetIsPop(bool isPop) {
	isPop_ = isPop;
}

void MeteoriteManager::KillAll() {
	for (auto& meteorite : meteorites_) {
		if (meteorite->GetIsAlive()) {
			meteorite->KillMe();
		}
	}
}
