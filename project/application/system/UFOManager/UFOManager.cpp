#include "UFOManager.h"
#include "framework/SUGER.h"

#include "random/Random.h"

#include "system/UFOBulletManager/UFOBulletManager.h"

void UFOManager::Initialize(UFOBulletManager* ufoBulletManager) {
	ufoBulletManager_ = ufoBulletManager;

	ufos_.clear();

	popTimer_ = popIntervalTime_;

	float screenWidth = 40.0f;
	float screenHeight = 22.5f;

	popPosition_[static_cast<uint32_t>(UFOPopPlace::LeftTop)] = { -screenWidth,screenHeight,0.0f };
	popPosition_[static_cast<uint32_t>(UFOPopPlace::RightTop)] = { screenWidth, screenHeight, 0.0f };
	popPosition_[static_cast<uint32_t>(UFOPopPlace::LeftBottom)] = { -screenWidth,-screenHeight,0.0f };
	popPosition_[static_cast<uint32_t>(UFOPopPlace::RightBottom)] = { screenWidth, -screenHeight, 0.0f };

}

void UFOManager::Update() {

	PopUFOs();

	// 削除フラグの立った隕石を削除
	ufos_.remove_if([](const std::unique_ptr<UFO>& ufo) {
		return !ufo->GetIsAlive();
		});

	// コンテナ内の隕石をすべて更新
	for (auto& ufo : ufos_) {
		ufo->Update();
	}
}

void UFOManager::AddUFO(const Vector3& popTranslate, const Vector3 velocity) {
	EulerTransform3D popTransform{};
	popTransform.translate = popTranslate;

	std::unique_ptr<UFO> newUfo = std::make_unique<UFO>();
	newUfo->Initialize(SUGER::CreateEntity("UFO", "UFO", popTransform));
	newUfo->CreateCollider(ColliderCategory::UFO, kSphere, 2.0f);
	newUfo->SetUFOBulletManager(ufoBulletManager_);
	newUfo->UpdateWorldTransform();
	newUfo->SetVelocity(velocity);
	newUfo->GetCollider()->SetMass(20000.0f);

	ufos_.push_back(std::move(newUfo));

	currentSerialNumber_++;
}

void UFOManager::AddColliderList() {
	for (auto& ufo : ufos_) {
		if (ufo->GetIsAlive()) {
			SUGER::AddColliderList(ufo.get());
		}

	}
}

void UFOManager::PopUFOs() {
	if (popTimer_ > 0) {
		popTimer_--;
	} else {
		for (int32_t i = 0; i < popNum_; i++) {
			Vector3 popPosition = {
				popPosition_[static_cast<uint32_t>(popPlace_)].x,
				popPosition_[static_cast<uint32_t>(popPlace_)].y + Random::GenerateFloat(-12.0f,12.0f),
				0.0f
			};

			// 場所によって初期ベクトルを設定
			switch (popPlace_) {
			case UFOPopPlace::LeftTop:
				AddUFO(popPosition, Vector3(1.0f, -1.0f, 0.0f));
				break;
			case UFOPopPlace::RightTop:
				AddUFO(popPosition, Vector3(-1.0f, -1.0f, 0.0f));
				break;
			case UFOPopPlace::LeftBottom:
				AddUFO(popPosition, Vector3(1.0f, 1.0f, 0.0f));
				break;
			case UFOPopPlace::RightBottom:
				AddUFO(popPosition, Vector3(-1.0f, 1.0f, 0.0f));
				break;
			}


			int popPosNum = Random::GenerateUint32_t(0, 3);
			popPlace_ = static_cast<UFOPopPlace>(popPosNum);
			popTimer_ = popIntervalTime_;
		}

	}
}

void UFOManager::SetPopIntervalTime(const uint32_t& popInterval) {
	popIntervalTime_ = popInterval;
}
