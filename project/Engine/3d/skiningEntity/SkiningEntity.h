#pragma once

#include "3d/entity/Entity.h"

class SkiningEntity :public Entity {
public:
	SkiningEntity() = default;
	~SkiningEntity() = default;

	// 描画
	void Update()override;
	// 描画
	void Draw()override;

};