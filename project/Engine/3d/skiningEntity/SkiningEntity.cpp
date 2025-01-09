#include "SkiningEntity.h"

#include "framework/SUGER.h"

void SkiningEntity::Update() {
	// 基底クラスの更新
	Entity::Update();
	// スキニング処理
	model_->Skinning();
}

void SkiningEntity::Draw() {
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
	model_->DrawSkinning();
	// コライダーがあれば描画
	if (collider_) {
		collider_->Draw();
	}
}
