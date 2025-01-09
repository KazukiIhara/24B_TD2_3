#include "Particle.h"

#include "framework/SUGER.h"

void Particle::Initialize(Model* model, Camera* camera, const std::string& textureFileName) {
	// インスタンスをセット
	SetModel(model);
	SetCamera(camera);

	// テクスチャファイル名をセット
	if (textureFileName != "") {
		textureFileName_ = textureFileName;
	}

	// マテリアル初期化
	material_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_.enableLighting = false;
	material_.shininess = 40.0f;
	material_.uvTransformMatrix = MakeIdentityMatrix4x4();

	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// マテリアルリソース作成
	CreateMaterialResource();
	// マテリアルデータ書き込み
	MapMaterialData();

	// srvのインデックスを割り当て
	srvIndex_ = SUGER::ViewAllocate();
	// Srvを作成
	SUGER::CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
}

void Particle::Update() {

	// 描画すべきインスタンス数
	instanceCount_ = 0;

	for (std::list<ParticleData>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();) {
		// 生存時間を過ぎていたら更新せず描画対象にしない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		// 経過時間を足す
		(*particleIterator).currentTime += kDeltaTime;
		// 移動
		(*particleIterator).transform.translate += kDeltaTime * (*particleIterator).velocity;
		// 透明度
		float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

		if (instanceCount_ < kNumMaxInstance) {
			// WVPマトリックスを求める
			// Scale
			Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
			// billboard(rotate)
			Matrix4x4 billboardMatrix = camera_->GetBillboardMatrix();
			
			// translate
			Matrix4x4 translateMatrix = MakeTranslateMatrix((*particleIterator).transform.translate);

			// ワールド行列を作成
			Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;

			// WVP行列を作成
			Matrix4x4 worldViewProjectionMatrix = worldMatrix * camera_->GetViewProjectionMatrix();

			instancingData_[instanceCount_].WVP = worldViewProjectionMatrix;
			instancingData_[instanceCount_].World = worldMatrix;
			// 色を入力
			instancingData_[instanceCount_].color.x = (*particleIterator).color.x;
			instancingData_[instanceCount_].color.y = (*particleIterator).color.y;
			instancingData_[instanceCount_].color.z = (*particleIterator).color.z;
			instancingData_[instanceCount_].color.w = alpha;

			// 生きているParticleの数を1つカウントする
			instanceCount_++;
		}
		// 次のイテレーターに進める
		++particleIterator;
	}
}

void Particle::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = SUGER::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(SUGER::GetPipelineState(kParticle, blendMode_));
	// StructuredBufferのSRVを設定する
	commandList->SetGraphicsRootDescriptorTable(1, SUGER::GetSRVUAVDescriptorHandleGPU(srvIndex_));
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// モデルがある場合描画
	if (model_) {
		switch (type_) {
			case kPlane:
				model_->DrawPlaneParticle(instanceCount_, textureFileName_);
				break;
			case kModel:
				model_->DrawModelParticle(instanceCount_);
				break;
		}
	}
}

void Particle::AddNewParticle(const EmitParamater& emitSetting) {

	// 最大数を超えていたら追加しない
	if (particles_.size() >= kNumMaxInstance) {
		return;
	}

	// 追加するパーティクル
	ParticleData particle;
	// トランスフォームの設定
	particle.transform.scale = emitSetting.scale;
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = emitSetting.position;
	// 移動量の設定
	particle.velocity = emitSetting.velocity;
	// 色の設定
	particle.color = emitSetting.color;

	// 生存時間の設定
	particle.lifeTime = emitSetting.lifeTime;
	particle.currentTime = 0;

	// コンテナに挿入
	particles_.push_back(particle);
}

void Particle::SetCamera(Camera* camera) {
	// カメラがなければassert
	assert(camera);
	camera_ = camera;
}

void Particle::SetType(ParticleType type) {
	type_ = type;
}

void Particle::SetBlendMode(const BlendMode& blendMode) {
	blendMode_ = blendMode;
}

void Particle::SetIsActive(const bool& isActive) {
	isActive_ = isActive;
}

const bool& Particle::GetIsActive() const {
	return isActive_;
}

void Particle::SetModel(Model* model) {
	// モデルがなければassert
	assert(model);
	model_ = model;
}

void Particle::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = SUGER::CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
}

void Particle::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Particle::CreateMaterialResource() {
	// マテリアル用のリソース作成
	materialResource_ = SUGER::CreateBufferResource(sizeof(Material3D));
}

void Particle::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}

