#pragma once

#include "objects/UFO/UFO.h"

#include <list>

class UFOManager {
public:

	void Initialize();

	void Update();

	void AddUFO(const Vector3& popTranslate, const Vector3& velocity);

	void AddColliderList();

	void PopUFOs();

private:
	// UFOリスト
	std::list<std::unique_ptr<UFO>> ufos_;

	// 
};