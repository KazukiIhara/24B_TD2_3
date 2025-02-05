#pragma once

#include "3d/entityController/EntityController.h"

#include <optional>

class Boss: public EntityController {
	enum class Behavior {
		kNone,
		kIn,
		kRoot,
	};
public:
	Boss();
	~Boss();

	void Initialize(const std::string& name)override;
	void Update()override;

	void RequestIn();
	void RequestRoot();

	void RootInitialize();
	void RootUpdate();

	void InInitialize();
	void InUpdate();

	void AddColliderList();
private:
	Behavior behavior_ = Behavior::kNone;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

};