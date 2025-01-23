#include "FragmentManager.h"

#include "framework/SUGER.h"

#include "objects/Earth/Earth.h"
#include "objects/player/Player.h"

#include "random/Random.h"

void FragmentManager::Initialize() {
	// コンテナをクリア
	fragments_.clear();


	float screenWidth = 40.0f;
	float screenHeight = 22.5f;
	popPosition_[Top] = { 0.0f,screenHeight,0.0f };
	popPosition_[LeftTop] = { -screenWidth,screenHeight,0.0f };
	popPosition_[Left] = { -screenWidth, 0.0f, 0.0f };
	popPosition_[LeftBottom] = { -screenWidth, -screenHeight, 0.0f };
	popPosition_[Bottom] = { 0.0f, -screenHeight, 0.0f };
	popPosition_[RightBottom] = { screenWidth, -screenHeight, 0.0f };
	popPosition_[Right] = { screenWidth, 0.0f, 0.0f };
	popPosition_[RightTop] = { screenWidth, screenHeight, 0.0f };


	popTimer_ = popIntervalTime_;

}
void FragmentManager::Update() {

#ifdef _DEBUG
	ImGui::Begin("FragmentManager");
	ImGui::Text("curentNum %d", currentSerialNumber_);
	ImGui::End();
#endif // _DEBUG

	PopFragments();

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
	newFragment->SetPlayer(player_);
	newFragment->SetEarth(earth_);
	newFragment->Initialize(SUGER::CreateEntity("Fragment", "Fragment", popTransform));
	newFragment->CreateCollider(ColliderCategory::Fragment, kSphere, 0.6f);
	newFragment->SetSerialNumber(currentSerialNumber_);
	newFragment->SetPraticle(currentSerialNumber_);

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
	if (popTimer_ > 0) {
		popTimer_--;
	} else {
		for (int32_t i = 0; i < popNum_; i++) {
			Vector3 popPosition = {
				popPosition_[popPlace_].x + Random::GenerateFloat(-2.0f,2.0f),
				popPosition_[popPlace_].y + Random::GenerateFloat(-2.0f,2.0f),
				0.0f
			};
			AddFragment(popPosition);

			int popPosNum = Random::GenerateUint32_t(0, 7);
			popPlace_ = static_cast<PopPlace>(popPosNum);
			popTimer_ = popIntervalTime_;
		}

	}
}
