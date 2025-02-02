#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Boss: public EntityController {
	enum class Behavior {
		kRoot,
		kCharge,
		kThrowMoon,
	};
public:
	Boss();
	~Boss();

private:

};