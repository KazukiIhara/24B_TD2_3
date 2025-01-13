#include "Meteorite.h"

#include "framework/SUGER.h"

void Meteorite::Initialize(const std::string& name) {
	EntityController::Initialize(name);



}

void Meteorite::SetEarth(Earth* earth) {
	assert(earth);
	earth_ = earth;
}

void Meteorite::Update() {
	
}

void Meteorite::OnCollision(Collider* other) {
	
}

void Meteorite::RootInitialize() {

}

void Meteorite::RootUpdate() {

}
