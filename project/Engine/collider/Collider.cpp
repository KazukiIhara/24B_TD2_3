#include "Collider.h"
#include "framework/SUGER.h"

void Collider::Initialize(WorldTransform* worldTransform, const ColliderCategory& colliderCategory, const ColliderType& colliderType, const float& size) {
	// 基底クラスの初期化
	Empty::Initialize();
	// 親トランスフォームのセット
	SetParent(worldTransform);
	// コライダーカテゴリーをセット
	SetColliderCategory(colliderCategory);
	// コライダータイプをセット
	SetColliderType(colliderType);
	// コライダーのサイズをセット
	SetSize(size);

	// コリジョンマネージャからラインコントローラをセット
	SetLineController(SUGER::GetColliderLineController());
}

void Collider::Draw() {
	DrawSphere(ExtractionWorldPos(worldTransform_.worldMatrix_), size_, 8);
}

void Collider::DrawSphere(const Vector3& center, float radius, uint32_t segments) {
	const int rings = segments;        // 緯度方向の分割数
	const int slices = segments * 2;  // 経度方向の分割数
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; // デフォルトの色 (白)

	// π の定数を取得
	const float pi = std::numbers::pi_v<float>;

	// 経度方向の線を作成
	for (int ring = 0; ring <= rings; ++ring) {
		float theta = pi * ring / rings;
		float y = center.y + radius * cosf(theta);
		float r = radius * sinf(theta);

		for (int slice = 0; slice < slices; ++slice) {
			float phi1 = 2.0f * pi * slice / slices;
			float phi2 = 2.0f * pi * (slice + 1) / slices;

			Vector3 p1 = { center.x + r * cosf(phi1), y, center.z + r * sinf(phi1) };
			Vector3 p2 = { center.x + r * cosf(phi2), y, center.z + r * sinf(phi2) };

			// 線を描画
			line_->DrawLine(p1, p2, color);
		}
	}

	// 緯度方向の線を作成
	for (int slice = 0; slice < slices; ++slice) {
		float phi = 2.0f * pi * slice / slices;

		for (int ring = 0; ring < rings; ++ring) {
			float theta1 = pi * ring / rings;
			float theta2 = pi * (ring + 1) / rings;

			Vector3 p1 = { center.x + radius * sinf(theta1) * cosf(phi), center.y + radius * cosf(theta1), center.z + radius * sinf(theta1) * sinf(phi) };
			Vector3 p2 = { center.x + radius * sinf(theta2) * cosf(phi), center.y + radius * cosf(theta2), center.z + radius * sinf(theta2) * sinf(phi) };

			// 線を描画
			line_->DrawLine(p1, p2, color);
		}
	}
}

void Collider::SetParent(WorldTransform* worldTransform) {
	assert(worldTransform);
	worldTransform_.parent_ = worldTransform;
}

void Collider::SetColliderType(const ColliderType& colliderType) {
	colliderType_ = colliderType;
}

void Collider::SetColliderCategory(const ColliderCategory& colliderCategory) {
	colliderCategory_ = colliderCategory;
}

void Collider::SetSize(const float& size) {
	size_ = size;
}

Vector3 Collider::GetWorldPosition() const {
	return ExtractionWorldPos(worldTransform_.worldMatrix_);
}

ColliderType Collider::GetColliderType() const {
	return colliderType_;
}

ColliderCategory Collider::GetColliderCategory() const {
	return colliderCategory_;
}

float Collider::GetSize() const {
	return size_;
}

void Collider::SetLineController(LineController* lineController) {
	assert(lineController);
	line_ = lineController;
}
