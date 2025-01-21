#include "FragmentManager.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"
#include "objects/player/Player.h"

void FragmentManager::Initialize() {
	// コンテナをクリア
	fragments_.clear();

}
void FragmentManager::Update() {
	// コンテナ内のかけらすべてを更新
	for (auto& fragment : fragments_) {
		fragment->Update();
	}

	fragments_.remove_if([](const std::unique_ptr<Fragment>& fragment) { 
		return !fragment->GetAlive(); 
		});

	
}


void FragmentManager::AddFragment(const Vector3& popTranslate) {

	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	// 新しいかけらを作成
	std::unique_ptr<Fragment> newFragment = std::make_unique<Fragment>();
	newFragment->Initialize(SUGER::CreateEntity("Fragment", "Fragment", popTransform));

	newFragment->SetPlayer(player_);

	newFragment->CreateCollider(ColliderCategory::Fragment, kSphere, 0.2f);
	newFragment->SetSerialNumber(currentSerialNumber_);
	newFragment->GetCollider()->SetMass(20.0f);
	// 追加
	fragments_.push_back(std::move(newFragment));

	// シリアルナンバーをインクリメント
	currentSerialNumber_++;

}

void FragmentManager::AddColliderList() {
	for (auto& fragment : fragments_) {
		SUGER::AddColliderList(fragment.get());
	}
}

void FragmentManager::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void FragmentManager::SetPlayer(Player* player) {
	assert(player);
	player_ = player;
}

void FragmentManager::PopFragments() {
	for (uint32_t i = 0; i < popNum_; i++) {
		AddFragment(Vector3(0.0f, 0.0f, 0.0f));
	}
}
