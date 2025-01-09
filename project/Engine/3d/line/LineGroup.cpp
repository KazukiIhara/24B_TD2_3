#include "LineGroup.h"
#include <cassert>

#include "framework/SUGER.h"

void LineGroup::Initialize(Camera* camera) {
	// カメラがなければassert
	assert(camera);
	SetCamera(camera);

	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// ViewProjectionリソースを作成
	CreateViewProjectionResource();
	// ViewProjectionデータを書き込む
	MapViewProjectionData();

	// srvのインデックスを割り当て
	srvIndex_ = SUGER::ViewAllocate();
	// Srvを作成
	SUGER::CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), kNumMaxInstance, sizeof(LineData3D));
}

void LineGroup::Update() {
	// ビュープロジェクションの更新
	viewProjectionData_->viewProjection = camera_->GetViewProjectionMatrix();

	// 描画すべきインスタンス数
	instanceCount_ = static_cast<uint32_t>(lines_.size());

	if (instancingData_ != nullptr) {
		for (uint32_t i = 0; i < instanceCount_; ++i) {
			// GPUにデータを転送
			instancingData_[i] = lines_[i];
		}
	}
	// ラインのコンテナをクリア
	ClearLines();
}

void LineGroup::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = SUGER::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(SUGER::GetPipelineState(kLine, blendMode_));
	// ViewProjectionを転送
	commandList->SetGraphicsRootConstantBufferView(0, viewProjectionResource_->GetGPUVirtualAddress());
	// StructuredBufferのSRVを設定する
	commandList->SetGraphicsRootDescriptorTable(1, SUGER::GetSRVUAVDescriptorHandleGPU(srvIndex_));
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(2, instanceCount_, 0, 0);
}

void LineGroup::AddLine(const Vector3& start, const Vector3& end, const Vector4& color) {
	// 最大数を超えていたら追加しない
	if (lines_.size() >= kNumMaxInstance) {
		return;
	}
	// 追加するLine
	LineData3D newLineData;
	newLineData.start = start;
	newLineData.end = end;
	newLineData.color = color;
	// コンテナに挿入
	lines_.push_back(newLineData);
}

void LineGroup::ClearLines() {
	lines_.clear();
}

void LineGroup::SetCamera(Camera* camera) {
	assert(camera);
	camera_ = camera;
}

void LineGroup::SetIsActive(const bool& isActive) {
	isActive_ = isActive;
}

void LineGroup::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = SUGER::CreateBufferResource(sizeof(LineData3D) * kNumMaxInstance);
}

void LineGroup::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void LineGroup::CreateViewProjectionResource() {
	// ViewProjection用のリソースを作る
	viewProjectionResource_ = SUGER::CreateBufferResource(sizeof(ViewProjectionForGPU));
}

void LineGroup::MapViewProjectionData() {
	// ViewProjection用のデータを書き込む
	viewProjectionData_ = nullptr;
	viewProjectionResource_->Map(0, nullptr, reinterpret_cast<void**>(&viewProjectionData_));
	viewProjectionData_->viewProjection = camera_->GetViewProjectionMatrix();
}
