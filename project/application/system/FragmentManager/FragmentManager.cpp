#include "FragmentManager.h"

#include "framework/SUGER.h"

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

void FragmentManager::AddFragment(const Vector3& popTranslate, Earth* earth) {

	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	// 新しいかけらを作成
	std::unique_ptr<Fragment> newFragment = std::make_unique<Fragment>();
	newFragment->Initialize(SUGER::CreateEntity("Fragment", "Fragment", popTransform));
	newFragment->SetEarth(earth);
	newFragment->CreateCollider(ColliderCategory::Fragment, kSphere, 0.2f);

	// 追加
	fragments_.push_back(std::move(newFragment));


}

void FragmentManager::AddColliderList() {
	for (auto& fragment : fragments_) {
		SUGER::AddColliderList(fragment.get());
	}
}
