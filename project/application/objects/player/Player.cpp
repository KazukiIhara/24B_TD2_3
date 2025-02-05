#include "Player.h"

#include "framework/SUGER.h"

#include "application/system/BumpManager/BumpManager.h"

#include "objects/Earth/Moon.h"

void Player::Initialize(const std::string& name) {
	EntityController::Initialize(name);
	localTransform_.Initialize();

	// プレイヤーのパラメータをグローバルデータグループに登録
	SUGER::AddGrobalDataGroup(kParamaterString);

	inclinationRadian_ = DegreesToRadians(inclination_);
	SetRotate(Vector3(0.0f, 0.0f, -inclinationRadian_));


	// 各パラメータをリストに登録
	SUGER::AddGrobalDataItem(kParamaterString, "Speed", kSpeed_);
	SUGER::AddGrobalDataItem(kParamaterString, "MaxSpeed", kMaxSpeed_);
	SUGER::AddGrobalDataItem(kParamaterString, "InitializePosition", initializePosition_);

	// グローバルデータからパラメータを取得
	SetParamaters();

	// 座標を初期化
	SetTranslate(Vector3(0.0f, 0.0f, 0.0f));

	earthHitTimer_ = 0;
	meteoriteHitTimer_ = 0;

	isAlive_ = true;

}

void Player::Update() {
#ifdef _DEBUG
	ImGui::Begin("PlayerOperation");
	ImGui::Text("MOVE: WASD");
	ImGui::Text("ROTATE: JK");
	ImGui::InputFloat3("velocity", &velocity_.x);
	ImGui::InputFloat3("acceralation", &acceralation_.x);
	ImGui::End();
#endif // =DEBUG

	// 自転
	SetRotateY(GetRotate().y + std::numbers::pi_v<float>*2.0f / aroundFrame_);
	HitTimersUpdate();
	SetParamaters();


	Operation();
	Move();
	MoveLimit();
	UpdateLifeState();

	localTransform_.translate_ = GetWorldTransformPtr()->translate_;
	localTransform_.Update();


	BehaviorUpdate();

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

	// キーボード操作
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

	if (SUGER::IsGamepadConnected(0)) {
		// パッド操作
		moveVector_.x = static_cast<float>(SUGER::GetLeftStickX(0));
		moveVector_.y = static_cast<float>(SUGER::GetLeftStickY(0));
	}

	// 移動ベクトル正規化
	moveVector_ = Normalize(moveVector_);

	// 
	// 射撃操作
	// 


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
}

void Player::MoveLimit() {
	Vector3 translate_ = GetTranslate();

	if (translate_.x > stageWidth_ ||
		translate_.x < -stageWidth_) {
		velocity_.x = 0.0f;
	}
	if (translate_.y > stageHeight_ ||
		translate_.y < -stageHeight_) {
		velocity_.y = 0.0f;
	}
	translate_.x = std::clamp(translate_.x, -stageWidth_, stageWidth_);
	translate_.y = std::clamp(translate_.y, -stageHeight_, stageHeight_);

	SetTranslate(translate_);
}

void Player::Shot() {
	moon_->AttackRequest();
	behaviorRequest_ = Behavior::kThrowMoon;
}

void Player::UpdateLifeState() {
	if (HP_ <= 0.0f) {
		isAlive_ = false;
		HP_ = 0.0f;
	}
}

void Player::OnCollision(Collider* other) {
	// 衝突相手のカテゴリーを取得
	ColliderCategory category = other->GetColliderCategory();
	// カテゴリごとに衝突判定を書く
	switch (category) {
		case ColliderCategory::Moon:

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
			isObjectHit = true;
			objectHitLevel = 2;
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

			// 隕石のバンプタイマーが0以下の場合、正規化された逆速度に基づいてバンプを追加します
			if (meteoriteBumpPopHitTimer_ <= 0.0f) {
				// 他のオブジェクトとのワールド座標の差を求め、正規化します
				bumpDirection_ = Normalize(other->GetWorldPosition() - GetCollider()->GetWorldPosition());

				Vector3 worldDirection = RotatePosition(bumpDirection_, -(GetRotate().z));

				// 変換後のベクトルを正規化してバンプを追加します
				//bumpManager_->AddBump(Normalize(worldDirection));

				// バンプタイマーをリセットします
				meteoriteBumpPopHitTimer_ = float(0.1f);
			}


			meteoriteHitTimer_ = kNoneHitTime_;
			HP_ -= 25;
		}
		break;
		case ColliderCategory::Fragment:
		{
			HP_ -= 2;

			isObjectHit = true;
			objectHitLevel = 1;
		}
		break;
		case ColliderCategory::UFOBullet:
		{
			HP_ -= 5;

			isObjectHit = true;
			objectHitLevel = 1;
		}
		break;

		case ColliderCategory::UFO:
		{
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

			isObjectHit = true;
			objectHitLevel = 2;

			HP_ -= 10;
		}
		break;

		case ColliderCategory::Boss:

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

			isObjectHit = true;
			objectHitLevel = 2;

			HP_ -= 20;


			break;
	}

}

Vector3 Player::RotatePosition(const Vector3& position, float angle) {
	float radians = angle;// * (3.1415926535f / 180.0f); // 度数法からラジアンへの変換
	float cosAngle = cos(radians);
	float sinAngle = sin(radians);

	float rotatedX = position.x * cosAngle - position.y * sinAngle;
	float rotatedY = position.x * sinAngle + position.y * cosAngle;
	float rotatedZ = position.z; // Z軸は回転しない

	return Vector3(rotatedX, rotatedY, rotatedZ);
}

WorldTransform* Player::GetLocalTransform() {
	return &localTransform_;
}


void Player::BehaviorUpdate() {
	// ふるまい変更
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
			case Player::Behavior::kRoot:
				RootInitialize();
				break;
			case Player::Behavior::kCharge:
				ChargeInitialize();
				break;
			case Player::Behavior::kThrowMoon:
				ThrowInitialize();
				break;
		}
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい
	switch (behavior_) {
		case Player::Behavior::kRoot:
			RootUpdate();
			break;
		case Player::Behavior::kCharge:
			ChargeUpdate();
			break;
		case Player::Behavior::kThrowMoon:
			ThrowUpdate();
			break;
	}
}

void Player::RootInitialize() {

}

void Player::RootUpdate() {
	if (SUGER::TriggerKey(DIK_SPACE) || SUGER::TriggerButton(0, ButtonA)) {
		behaviorRequest_ = Behavior::kCharge;
	}

	// 月を回す処理
	localTransform_.rotate_.z += rotationSpeed_ * SUGER::kDeltaTime_;
}

void Player::ChargeInitialize() {
	moon_->ChargeRequest();
}

void Player::ChargeUpdate() {
	if (SUGER::ReleaseKey(DIK_SPACE) || SUGER::ReleaseButton(0, ButtonA)) {
		Shot();
	}
}

void Player::ThrowInitialize() {
	localTransform_.rotate_.z = 0.0f;
	catchTimer_ = catchTime_;
}

void Player::ThrowUpdate() {

	if (SUGER::PushKey(DIK_SPACE) || SUGER::PushButton(0, ButtonA)) {
		moon_->BackUpdate();
	}

	if (catchTimer_ > 0) {
		catchTimer_--;
	}

	Vector3 worldPosition = ExtractionWorldPos(localTransform_.worldMatrix_);
	Vector3 moonPosition = ExtractionWorldPos(moon_->GetWorldTransformPtr()->worldMatrix_);
	Vector3 subtract = moonPosition - worldPosition;
	Vector3 direction = Normalize(subtract);
	float distance = Length(subtract);

	if (distance < moonCatchDistance_ && !catchTimer_) {
		moon_->RootRequest();
		moon_->SetTranslate(direction * (distance + moonRotateDistanceOffset_));
		moon_->UpdateWorldTransform();
		moon_->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
		behaviorRequest_ = Behavior::kRoot;
	}
}

float& Player::GetHp() {
	return HP_;

}

