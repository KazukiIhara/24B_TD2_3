#pragma once

#include "3d/line/LineGroup.h"

class LineController {
public:
	LineController() = default;
	~LineController() = default;

	// 初期化
	void Initialize(const std::string& name);

	// 描画するラインを追加
	void DrawLine(const Vector3& start, const Vector3& end, const Vector4& color);

private:
	// Line
	LineGroup* line_ = nullptr;

};