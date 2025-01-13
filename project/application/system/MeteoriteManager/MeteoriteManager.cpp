#include "MeteoriteManager.h"

#include "framework/SUGER.h"

void MeteoriteManager::Initialize(Earth* earth,FragmentManager* fragmentManager) {
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

	kMeteritePopScale_ = SUGER::GetGrobalDataValueVector3(kParamaterString_, "PopScale");
	kMeteritePopRotate_ = SUGER::GetGrobalDataValueVector3(kParamaterString_, "PopRotate");
	kMeteritePopTranslate_ = SUGER::GetGrobalDataValueVector3(kParamaterString_, "PopPosition");

	// 受け取ったグローバルデータを挿入
	meteoritePopTransform_.scale = kMeteritePopScale_;
	meteoritePopTransform_.rotate = kMeteritePopRotate_;
	meteoritePopTransform_.translate = kMeteritePopTranslate_;
}

void MeteoriteManager::Update() {
	// コンテナ内の隕石をすべて更新
	for (auto& meteorite : meteorites_) {
		meteorite->Update();
	}
}

void MeteoriteManager::AddMeteorite() {
	// 新しい隕石を作成
	std::unique_ptr<Meteorite> newMeteorite = std::make_unique<Meteorite>();
	newMeteorite->Initialize(SUGER::CreateEntity("Meteorite", "Meteorite", meteoritePopTransform_));
	newMeteorite->SetEarth(earth_);
	// 追加
	meteorites_.push_back(std::move(newMeteorite));
}
