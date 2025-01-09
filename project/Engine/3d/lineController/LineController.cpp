#include "LineController.h"

#include "framework/SUGER.h"

void LineController::Initialize(const std::string& name) {
	line_ = SUGER::FindLine(name);
	assert(line_);
}

void LineController::DrawLine(const Vector3& start, const Vector3& end, const Vector4& color) {
	line_->AddLine(start, end, color);
}
