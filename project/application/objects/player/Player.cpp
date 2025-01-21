#include "Player.h"

#include "framework/SUGER.h"

#include "application/system/BumpManager/BumpManager.h"

void Player::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	// プレイヤーのパラメータをグローバルデータグループに登録
	SUGER::AddGrobalDataGroup(kParamaterString);

	// 各パラメータをリストに登録
	SUGER::AddGrobalDataItem(kParamaterString, "Speed", kSpeed_);
	SUGER::AddGrobalDataItem(kParamaterString, "MaxSpeed", kMaxSpeed_);
	SUGER::AddGrobalDataItem(kParamaterString, "InitializePosition", initializePosition_);

	// グローバルデータからパラメータを取得
	SetParamaters();

	// 座標を初期化
	SetTranslate(initializePosition_);

	earthHitTimer_ = 0;
	meteoriteHitTimer_ = 0;
}

void Player::Update() {
#ifdef _DEBUG
	ImGui::Begin("PlayerOperation");
	ImGui::Text("MOVE: WASD");
	ImGui::Text("ROTATE: JK");
	ImGui::End();
#endif // =DEBUG

	HitTimersUpdate();
	SetParamaters();
	Operation();
	Move();
	MoveLimit();

	

}

void Player::SetParamaters() {
	kSpeed_ = SUGER::GetGrobalDataValueFloat(kParamaterString, "Speed");
	kMaxSpeed_ = SUGER::GetGrobalDataValueFloat(kParamaterString, "MaxSpeed");
	initializePosition_ = SUGER::GetGrobalDataValueVector3(kParamaterString, "InitializePosition");
}

void Player::HitTimersUpdate() {
	if (earthHitTimer_ > 0) {
		earthHitTimer_--;
	}
	if (meteoriteHitTimer_ > 0) {
		meteoriteHitTimer_--;
	}
	if (meteoriteBumpPopHitTimer_ > 0) {
		meteoriteBumpPopHitTimer_ -= SUGER::kDeltaTime_;
	}
}

void Player::Operation() {
	// 移動方向を初期化
	moveVector_ = { 0.0f,0.0f,0.0f };
	rotateDirection_ = 0.0f;

	if (SUGER::PushKey(DIK_W)) {
		moveVector_.y += 1.0f;
	}
	if (SUGER::PushKey(DIK_A)) {
		moveVector_.x -= 1.0f;
	}
	if (SUGER::PushKey(DIK_S)) {
		moveVector_.y -= 1.0f;
	}
	if (SUGER::PushKey(DIK_D)) {
		moveVector_.x += 1.0f;
	}
	// 移動ベクトル正規化
	moveVector_ = Normalize(moveVector_);

	if (SUGER::PushKey(DIK_J)) {
		rotateDirection_ += 1.0f;
	}
	if (SUGER::PushKey(DIK_K)) {
		rotateDirection_ -= 1.0f;
	}
}

void Player::Move() {
	// 移動ベクトルと速度を加速量に反映
	acceralation_ = moveVector_ * kSpeed_;
	// 移動量に加速量を足す
	velocity_ += acceralation_;
	// 最大速度でクランプ
	velocity_.x = std::clamp(velocity_.x, -kMaxSpeed_, kMaxSpeed_);
	velocity_.y = std::clamp(velocity_.y, -kMaxSpeed_, kMaxSpeed_);
	velocity_.z = std::clamp(velocity_.z, -kMaxSpeed_, kMaxSpeed_);
	// 移動量を足す
	SetTranslate(GetTranslate() + velocity_ * SUGER::kDeltaTime_);
	// 現在の移動量をセット
	GetCollider()->SetVelocity(velocity_);

	// 回転
	SetRotateZ(GetRotate().z + rotateDirection_ * rotationSpeed_);

}

void Player::MoveLimit() {
	Vector3 translate_ = GetTranslate();
	translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
	translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);
	SetTranslate(translate_);
}

void Player::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
		case ColliderCategory::Earth:

		{
			//
			// 押し戻し処理(強引)
			// 

			// 位置ベクトルを取得
			Vector3 posA = GetCollider()->GetWorldPosition();
			Vector3 posB = other->GetWorldPosition();
			// 各オブジェクトの「半径」相当の値を取得 (球体などの場合)
			float radiusA = GetCollider()->GetSize();
			float radiusB = other->GetSize();
			// 合計半径
			float sumRadius = radiusA + radiusB;
			// ２つのオブジェクト間の距離
			Vector3 diff = posA - posB;
			float distance = Length(diff);

			Vector3 normal = Normalize(posA - posB);
			if (distance < sumRadius) {
				SetTranslate(other->GetWorldPosition() + normal * (sumRadius + 0.1f));
			}


			float playerMass = GetCollider()->GetMass();
			Vector3 playerVelocity = GetCollider()->GetVelocity();
			float  earthMass = other->GetMass();
			Vector3 earthVelocity = other->GetVelocity();
			Vector3 velocity = ComputeCollisionVelocity(playerMass, playerVelocity, earthMass, earthVelocity, 1.0f, normal);
			velocity_ = velocity;
			earthHitTimer_ = kNoneHitTime_;
		}
		break;
		case::ColliderCategory::Meteorite:
		{

			//
			// 押し戻し処理(強引)
			// 

			// 位置ベクトルを取得
			Vector3 posA = GetCollider()->GetWorldPosition();
			Vector3 posB = other->GetWorldPosition();
			// 各オブジェクトの「半径」相当の値を取得 (球体などの場合)
			float radiusA = GetCollider()->GetSize();
			float radiusB = other->GetSize();
			// 合計半径
			float sumRadius = radiusA + radiusB;
			// ２つのオブジェクト間の距離
			Vector3 diff = posA - posB;
			float distance = Length(diff);

			Vector3 normal = Normalize(posA - posB);
			if (distance < sumRadius) {
				SetTranslate(other->GetWorldPosition() + normal * (sumRadius + 0.1f));
			}

			float playerMass = GetCollider()->GetMass();
			Vector3 playerVelocity = GetCollider()->GetVelocity();
			float meteoriteMass = other->GetMass();
			Vector3 meteoriteVelocity = other->GetVelocity();
			Vector3 velocity = ComputeCollisionVelocity(playerMass, playerVelocity, meteoriteMass, meteoriteVelocity, 1.0f, normal);
			velocity_ = velocity;
			
			if (meteoriteBumpPopHitTimer_ <= 0.0f) {
				bumpManager_->AddBump(Normalize(-velocity));
			}
			meteoriteHitTimer_ = kNoneHitTime_;
			meteoriteBumpPopHitTimer_ = float(kNoneHitTime_);
		}
		break;

	}

}
