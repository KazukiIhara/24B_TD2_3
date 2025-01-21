#include "MeteoriteManager.h"

#include "framework/SUGER.h"

#include "system/FragmentManager/FragmentManager.h"
#include "objects/Earth/Earth.h"

void MeteoriteManager::Initialize(Earth* earth, FragmentManager* fragmentManager) {
	assert(earth);
	assert(fragmentManager);
	earth_ = earth;
	fragmentManager_ = fragmentManager;


	// コンテナをクリア
	meteorites_.clear();

	// グローバルデータグループを作成
	SUGER::AddGrobalDataGroup(kParamaterString_);
	// グローバルデータを挿入
	SUGER::AddGrobalDataItem(kParamaterString_, "PopScale", kMeteritePopScale_);
	SUGER::AddGrobalDataItem(kParamaterString_, "PopRotate", kMeteritePopRotate_);
	SUGER::AddGrobalDataItem(kParamaterString_, "PopPosition", kMeteritePopTranslate_);
	SUGER::AddGrobalDataItem(kParamaterString_, "Speed", speed_);


	kMeteritePopScale_ = SUGER::GetGrobalDataValueVector3(kParamaterString_, "PopScale");
	kMeteritePopRotate_ = SUGER::GetGrobalDataValueVector3(kParamaterString_, "PopRotate");
	kMeteritePopTranslate_ = SUGER::GetGrobalDataValueVector3(kParamaterString_, "PopPosition");
	speed_ = SUGER::GetGrobalDataValueFloat(kParamaterString_, "Speed");

	// 受け取ったグローバルデータを挿入
	meteoritePopTransform_.scale = kMeteritePopScale_;
	meteoritePopTransform_.rotate = kMeteritePopRotate_;
	meteoritePopTransform_.translate = kMeteritePopTranslate_;
}

void MeteoriteManager::Update() {
	// グローバルデータクラスからデータを取得
	speed_ = SUGER::GetGrobalDataValueFloat(kParamaterString_, "Speed");

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

void MeteoriteManager::AddMeteorite() {
	// 新しい隕石を作成
	std::unique_ptr<Meteorite> newMeteorite = std::make_unique<Meteorite>();
	newMeteorite->Initialize(SUGER::CreateEntity("Meteorite", "Meteorite", meteoritePopTransform_));
	newMeteorite->SetEarth(earth_);
	newMeteorite->SetFragmentManager(fragmentManager_);
	newMeteorite->CreateCollider(ColliderCategory::Meteorite, kSphere, 4.0f);

	newMeteorite->GetCollider()->SetMass(20000.0f);

	// 追加
	meteorites_.push_back(std::move(newMeteorite));
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
