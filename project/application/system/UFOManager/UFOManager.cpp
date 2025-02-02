#include "UFOManager.h"

void UFOManager::Initialize() {

}

void UFOManager::Update() {

	PopUFOs();

	// 削除フラグの立った隕石を削除
	ufos_.remove_if([](const std::unique_ptr<UFO>& meteorite) {
		return !meteorite->GetIsAlive();
		});

	// コンテナ内の隕石をすべて更新
	for (auto& ufo : ufos_) {
		ufo->Update();
	}
}

void UFOManager::AddUFO(const Vector3& popTranslate, const Vector3& velocity) {

}

void UFOManager::AddColliderList() {

}

void UFOManager::PopUFOs() {

}