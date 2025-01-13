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
}

void FragmentManager::AddFragment(const Vector3& popTranslate) {

	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	// 新しいかけらを作成
	std::unique_ptr<Fragment> newFragment = std::make_unique<Fragment>();
	newFragment->Initialize(SUGER::CreateEntity("Meteorite", "Meteorite", popTransform));
	// 追加
	fragments_.push_back(std::move(newFragment));
}
