#include "Entity.h"

#include "framework/SUGER.h"

void Entity::Initialize() {
	// 基底クラスの初期化処理
	Empty::Initialize();

	// マテリアル初期化
	material_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_.enableLighting = true;
	material_.shininess = 40.0f;
	material_.uvTransformMatrix = MakeIdentityMatrix4x4();

	// WVP用のリソース作成
	CreateWVPResource();
	// データを書きこむ
	MapWVPData();
	// マテリアル用のリソース作成
	CreateMaterialResource();
	// マテリアルデータ書き込み
	MapMateiralData();
}

void Entity::Update() {
	// モデルの更新
	model_->Update();
	// 基底クラスの更新処理
	Empty::Update();

	// コライダーが有効ならコライダーの更新
	if (collider_) {
		collider_->Update();
	}

	// WVPマトリックス作成
	transformationData_->World = worldTransform_.worldMatrix_;
	transformationData_->ViewProjection = camera_->GetViewProjectionMatrix();
	transformationData_->WorldInverseTransepose = MakeInverseTransposeMatrix(worldTransform_.worldMatrix_);

	// マテリアルの更新
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}

void Entity::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = SUGER::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(SUGER::GetPipelineState(GraphicsPipelineStateType::kObject3d, blendMode_));
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());
	// ライトを転送
	punctualLight_->TransferLight();
	// カメラ情報を転送
	camera_->TransferCamera(3);
	// 3Dモデル描画
	model_->Draw();
	// コライダーがあれば描画
	if (collider_) {
		collider_->Draw();
	}
}

void Entity::CreateCollider(const ColliderCategory& colliderCategory, const ColliderType& colliderType, const float& size) {
	collider_ = std::make_unique<Collider>();
	collider_->Initialize(&worldTransform_, colliderCategory, colliderType, size);
}

Collider* Entity::GetCollider() {
	return collider_.get();
}

Camera* Entity::GetCamera() {
	return camera_;
}

void Entity::SetModel(const std::string& filePath) {
	// モデルをセット
	model_ = SUGER::FindModel(filePath);
	// モデルがなければassert
	assert(model_);
}

void Entity::SetCamera(Camera* camera) {
	// カメラがなければassert
	assert(camera);
	camera_ = camera;
}

void Entity::SetLight(PunctualLight* punctualLight) {
	// ライトがなければassert
	assert(punctualLight);
	punctualLight_ = punctualLight;
}

void Entity::SetColor(const Vector4& color) {
	material_.color = color;
}

void Entity::SetEnableLight(const bool& enableLight) {
	material_.enableLighting = enableLight;
}

void Entity::SetShininess(const float& shininess) {
	material_.shininess = shininess;
}

void Entity::CreateWVPResource() {
	// WVP用のリソースを作る
	transformationResource_ = SUGER::CreateBufferResource(sizeof(TransformationMatrix));
}

void Entity::MapWVPData() {
	// データを書き込む
	transformationData_ = nullptr;
	// 書き込むためのアドレスを取得
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	// 単位行列を書き込んでおく
	transformationData_->World = MakeIdentityMatrix4x4();
	transformationData_->ViewProjection = MakeIdentityMatrix4x4();
	transformationData_->WorldInverseTransepose = MakeIdentityMatrix4x4();
}

void Entity::CreateMaterialResource() {
	// マテリアル用のリソース作成
	materialResource_ = SUGER::CreateBufferResource(sizeof(Material3D));
}

void Entity::MapMateiralData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}
