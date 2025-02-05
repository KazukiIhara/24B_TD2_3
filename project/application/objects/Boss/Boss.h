#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Boss: public EntityController {
	enum class Behavior {
		kRoot,
	};
public:
	Boss();
	~Boss();

	void Initialize(const std::string& name)override;

private:

};