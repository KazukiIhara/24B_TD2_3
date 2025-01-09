#include "SUGER.h"

#include "debugTools/logger/Logger.h"

#ifdef _DEBUG
std::unique_ptr<D3DResourceLeakChecker> SUGER::leakCheck_ = nullptr;
#endif // _DEBUG

// Staticメンバ変数の初期化
std::unique_ptr<WindowManager> SUGER::windowManager_ = nullptr;
std::unique_ptr<DirectInput> SUGER::directInput_ = nullptr;
std::unique_ptr<FixFPS> SUGER::fixFPS_ = nullptr;

std::unique_ptr<DXGIManager> SUGER::dxgiManager_ = nullptr;
std::unique_ptr<DirectXCommand> SUGER::command_ = nullptr;
std::unique_ptr<Fence> SUGER::fence_ = nullptr;

std::unique_ptr<RTVManager> SUGER::rtvManager_ = nullptr;
std::unique_ptr<DSVManager> SUGER::dsvmanager_ = nullptr;
std::unique_ptr<SRVUAVManager> SUGER::srvUavManager_ = nullptr;

std::unique_ptr<TextureManager> SUGER::textureManager_ = nullptr;
std::unique_ptr<ModelManager> SUGER::modelManager_ = nullptr;
std::unique_ptr<SoundManager> SUGER::soundManager_ = nullptr;

std::unique_ptr<Object2DManager> SUGER::object2dManager_ = nullptr;
std::unique_ptr<EmptyManager> SUGER::emptyManager_ = nullptr;
std::unique_ptr<EntityManager> SUGER::entityManager_ = nullptr;
std::unique_ptr<EmitterManager> SUGER::emitterManager_ = nullptr;
std::unique_ptr<ParticleManager> SUGER::particleManager_ = nullptr;
std::unique_ptr<LineGroupManager> SUGER::lineManager_ = nullptr;

std::unique_ptr<GraphicsPipelineManager> SUGER::graphicsPipelineManager_ = nullptr;
std::unique_ptr<ComputePipelineManager> SUGER::computePipelineManager_ = nullptr;
std::unique_ptr<PostEffectPipelineManager> SUGER::postEffectPipelineManager_ = nullptr;

std::unique_ptr<JsonLevelDataManager> SUGER::jsonLevelDataManager_ = nullptr;
std::unique_ptr<GrobalDataManager> SUGER::grobalDataManager_ = nullptr;

std::unique_ptr<CollisionManager> SUGER::collisionManager_ = nullptr;
std::unique_ptr<ImGuiManager> SUGER::imguiManager_ = nullptr;

std::unique_ptr<SwapChain> SUGER::swapChain_ = nullptr;
std::unique_ptr<RenderTexture> SUGER::renderTexture_ = nullptr;
std::unique_ptr<DepthStencil> SUGER::depthStencil_ = nullptr;
std::unique_ptr<Barrier> SUGER::barrier_ = nullptr;
std::unique_ptr<TargetRenderPass> SUGER::targetRenderPass_ = nullptr;
std::unique_ptr<ViewPort> SUGER::viewPort_ = nullptr;
std::unique_ptr<ScissorRect> SUGER::scissorRect_ = nullptr;

std::unique_ptr<Object2DSystem> SUGER::object2dSystem_ = nullptr;
std::unique_ptr<Object3DSystem> SUGER::object3dSystem_ = nullptr;
std::unique_ptr<ParticleSystem> SUGER::particleSystem_ = nullptr;
std::unique_ptr<LineSystem> SUGER::lineSystem_ = nullptr;

void SUGER::Initialize() {
	// フレームワーク初期化ログ
	Logger::Log("SUGER,Initialize\n");

#pragma region BaseSystems
	// WindowManagerの初期化
	windowManager_ = std::make_unique<WindowManager>();
	windowManager_->Initialize();
	// DirectInputの初期化
	directInput_ = std::make_unique<DirectInput>();
	directInput_->Initialize(windowManager_.get());
	// FixFPS
	fixFPS_ = std::make_unique<FixFPS>();
	fixFPS_->Initialize();
#pragma endregion

#pragma region DirectXBaseSystems
	// DXGIManagerの初期化
	dxgiManager_ = std::make_unique<DXGIManager>();
	dxgiManager_->Initialize();
	// Commandの初期化
	command_ = std::make_unique<DirectXCommand>();
	command_->Initialize(dxgiManager_.get());
	// Fenceの初期化
	fence_ = std::make_unique<Fence>();
	fence_->Initialize(dxgiManager_.get(), command_.get());
#pragma endregion

#pragma region ViewManagers
	// RTVManagerの初期化
	rtvManager_ = std::make_unique<RTVManager>();
	rtvManager_->Initialize(dxgiManager_.get());
	// DSVManagerの初期化
	dsvmanager_ = std::make_unique<DSVManager>();
	dsvmanager_->Initialize(dxgiManager_.get(), command_.get());
	// SRVUAVManagerの初期化
	srvUavManager_ = std::make_unique<SRVUAVManager>();
	srvUavManager_->Initialize(dxgiManager_.get(), command_.get());
#pragma endregion

#pragma region ResourceManagers
	// TextureManagerの初期化
	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Initialize(dxgiManager_.get(), command_.get(), fence_.get(), srvUavManager_.get());
	// ModelManagerの初期化
	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Initialize();
	// soundManagerの初期化
	soundManager_ = std::make_unique<SoundManager>();
	soundManager_->Initialize();
#pragma endregion

#pragma region ObjectManagers
	// object2dManagerの初期化
	object2dManager_ = std::make_unique<Object2DManager>();
	object2dManager_->Initialize();
	// emptyManagerの初期化
	emptyManager_ = std::make_unique<EmptyManager>();
	emptyManager_->Initialize();
	// entityManagerの初期化
	entityManager_ = std::make_unique<EntityManager>();
	entityManager_->Initialize(modelManager_.get());
	// emitterManagerの初期化
	emitterManager_ = std::make_unique<EmitterManager>();
	emitterManager_->Initialize();
	// ParticleManagerの初期化
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize(modelManager_.get(), textureManager_.get());
	// LineManagerの初期化
	lineManager_ = std::make_unique<LineGroupManager>();
	lineManager_->Initialize();
#pragma endregion

#pragma region PipeLineManagers
	// GraphicsPipelineManagerの初期化
	graphicsPipelineManager_ = std::make_unique<GraphicsPipelineManager>();
	graphicsPipelineManager_->Initialize(dxgiManager_.get());
	// ComputePipelineManagerの初期化
	computePipelineManager_ = std::make_unique<ComputePipelineManager>();
	computePipelineManager_->Initialize(dxgiManager_.get());
	// PostEffectPipelineManagerの初期化
	postEffectPipelineManager_ = std::make_unique<PostEffectPipelineManager>();
	postEffectPipelineManager_->Initialize(dxgiManager_.get());
#pragma endregion

#pragma region DataManagers
	// JsonLevelDataManagerの初期化
	jsonLevelDataManager_ = std::make_unique<JsonLevelDataManager>();
	jsonLevelDataManager_->Initialize();
	// grobalDataManagerの初期化
	grobalDataManager_ = std::make_unique<GrobalDataManager>();
	grobalDataManager_->Initialize(fixFPS_.get());
#pragma endregion

#pragma region SystemManagers
	// collisionManager
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(lineManager_.get());
	// ImGuiManagerの初期化
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Initialize(windowManager_.get(), dxgiManager_.get(), command_.get(), srvUavManager_.get());
#pragma endregion

#pragma region DirectXRenderSystems
	// SwapChainの初期化
	swapChain_ = std::make_unique<SwapChain>();
	swapChain_->Initialize(windowManager_.get(), dxgiManager_.get(), command_.get(), rtvManager_.get());
	// RenderTextureの初期化
	renderTexture_ = std::make_unique<RenderTexture>();
	renderTexture_->Initialize(dxgiManager_.get(), command_.get(), rtvManager_.get(), srvUavManager_.get(),postEffectPipelineManager_.get());
	// DepthStencilの初期化
	depthStencil_ = std::make_unique<DepthStencil>();
	depthStencil_->Initialize(dxgiManager_.get(), command_.get(), dsvmanager_.get());
	// Barrierの初期化
	barrier_ = std::make_unique<Barrier>();
	barrier_->Initialize(command_.get(), swapChain_.get(), renderTexture_.get());
	// TargetRenderPassの初期化
	targetRenderPass_ = std::make_unique<TargetRenderPass>();
	targetRenderPass_->Initialize(command_.get(), swapChain_.get(), renderTexture_.get(), depthStencil_.get());
	// Viewportの初期化
	viewPort_ = std::make_unique<ViewPort>();
	viewPort_->Initialize(command_.get());
	// ScissorRectの初期化
	scissorRect_ = std::make_unique<ScissorRect>();
	scissorRect_->Initialize(command_.get());
#pragma endregion

#pragma region Systems
	// Object2DSystemの初期化
	object2dSystem_ = std::make_unique<Object2DSystem>();
	object2dSystem_->Initialize(command_.get(), graphicsPipelineManager_.get());
	// Object3DSystemの初期化
	object3dSystem_ = std::make_unique<Object3DSystem>();
	object3dSystem_->Initialize(command_.get(), graphicsPipelineManager_.get());
	// ParticleSystemの初期化
	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize(command_.get(), graphicsPipelineManager_.get());
	// LineSystemの初期化
	lineSystem_ = std::make_unique<LineSystem>();
	lineSystem_->Initialize(command_.get(), graphicsPipelineManager_.get());
#pragma endregion
}

void SUGER::Finalize() {
	Logger::Log("SUGER,Finalized\n");

	// 各オブジェクトの終了処理を生成した順番とは逆に行う

	// LineSystemの終了処理
	if (lineSystem_) {
		lineSystem_.reset();
	}

	// ParticleSystemの終了処理
	if (particleSystem_) {
		particleSystem_.reset();
	}

	// Object3DSystemの終了処理
	if (object3dSystem_) {
		object3dSystem_.reset();
	}

	// Object2DSystemの終了処理
	if (object2dSystem_) {
		object2dSystem_.reset();
	}

	// ScissorRectの終了処理
	if (scissorRect_) {
		scissorRect_.reset();
	}

	// Viewportの終了処理
	if (viewPort_) {
		viewPort_.reset();
	}

	// TargetRenderPassの終了処理
	if (targetRenderPass_) {
		targetRenderPass_.reset();
	}

	// Barrierの終了処理
	if (barrier_) {
		barrier_.reset();
	}

	// DepthStencilの終了処理
	if (depthStencil_) {
		depthStencil_.reset();
	}

	// RenderTextureの終了処理
	if (renderTexture_) {
		renderTexture_.reset();
	}

	// SwapChainの終了処理
	if (swapChain_) {
		swapChain_.reset();
	}

	// ImGuiManagerの終了処理
	if (imguiManager_) {
		imguiManager_->Finalize();
		imguiManager_.reset();
	}

	// collisionManagerの終了処理
	if (collisionManager_) {
		collisionManager_->Finalize();
		collisionManager_.reset();
	}

	// grobalDataManagerの終了処理
	if (grobalDataManager_) {
		grobalDataManager_->Finalize();
		grobalDataManager_.reset();
	}

	// JsonLevelDataManagerの終了処理
	if (jsonLevelDataManager_) {
		jsonLevelDataManager_->Finalize();
		jsonLevelDataManager_.reset();
	}

	// PostEffectPipelineManagerの終了処理
	if (postEffectPipelineManager_) {
		postEffectPipelineManager_.reset();
	}

	// ComputePipelineManagerの終了処理
	if (computePipelineManager_) {
		computePipelineManager_.reset();
	}

	// GraphicsPipelineManagerの終了処理
	if (graphicsPipelineManager_) {
		graphicsPipelineManager_.reset();
	}

	// emitterManagerの終了処理
	if (emitterManager_) {
		emitterManager_->Finalize();
		emitterManager_.reset();
	}

	// ParticleManagerの終了処理
	if (particleManager_) {
		particleManager_->Finalize();
		particleManager_.reset();
	}

	// EmptyManagerの終了処理
	if (emptyManager_) {
		emptyManager_->Finalize();
		emptyManager_.reset();
	}

	// EntityManagerの終了処理
	if (entityManager_) {
		entityManager_->Finalize();
		entityManager_.reset();
	}

	// Object2DManagerの終了処理
	if (object2dManager_) {
		object2dManager_->Finalize();
		object2dManager_.reset();
	}

	// soundManagerの終了処理
	if (soundManager_) {
		soundManager_->Finalize();
		soundManager_.reset();
	}

	// ModelManagerの終了処理
	if (modelManager_) {
		modelManager_.reset();
	}

	// TextureManagerの終了処理
	if (textureManager_) {
		textureManager_.reset();
	}

	// SrvUavManagerの終了処理
	if (srvUavManager_) {
		srvUavManager_.reset();
	}

	// DSVManagerの終了処理
	if (dsvmanager_) {
		dsvmanager_.reset();
	}

	// RTVManagerの終了処理
	if (rtvManager_) {
		rtvManager_.reset();
	}

	// fenceの終了処理
	if (fence_) {
		fence_.reset();
	}

	// commandの終了処理
	if (command_) {
		command_.reset();
	}

	// DXGIManagerの終了処理
	if (dxgiManager_) {
		dxgiManager_.reset();
	}

	// FixFPSの終了処理
	if (fixFPS_) {
		fixFPS_.reset();
	}

	// inputの終了処理
	if (directInput_) {
		directInput_.reset();
	}

	// WindowManagerの終了処理
	if (windowManager_) {
		windowManager_->Finalize();
		windowManager_.reset();
	}
}

void SUGER::Update() {

	// ウィンドウマネージャの更新処理
	windowManager_->Update();

	// ウィンドウにメッセージが来ていたら最優先で処理
	if (windowManager_->ProcessMessage()) {
		endRequest_ = true;
	}

	// 入力の更新
	directInput_->Update();

	// FixFPSの更新
	fixFPS_->Update();

	// F11キーでフルスクリーン切り替え処理
	if (directInput_->TriggerKey(DIK_F11)) {
		windowManager_->ToggleFullScreen();
	}

	// escキーで終了
	if (directInput_->PushKey(DIK_ESCAPE)) {
		endRequest_ = true;
	}

	// 再生の終わっている音声を再生中コンテナから削除
	soundManager_->CleanupFinishedVoices();

	// ImGui開始処理
	imguiManager_->BeginFrame();

#ifdef _DEBUG
	// グローバルデータ更新処理
	grobalDataManager_->Update();
	// FPS表示
	ShowFPS();
#endif // _DEBUG

}

void SUGER::Draw() {
	// 3Dオブジェクト描画前処理
	PreDrawObject3D();
	// Entity描画処理
	DrawEntiteis();
	// Skining付きEntity描画処理
	DrawSkiningEntities();

	// 3Dパーティクル描画前処理
	PreDrawParticle3D();
	// 3Dパーティクル描画処理
	DrawParticles();

	// 3DLine描画前処理
	PreDrawLine3D();
	// 3DLine描画処理
	DrawLines();

	// 2Dオブジェクト描画前処理
	PreDrawObject2D();
	// 2Dオブジェクト描画処理
	Draw2DObjects();
}

void SUGER::Run() {
	// 初期化
	Initialize();
	// メインループ
	while (true) {
		// 更新
		Update();

		// 終了リクエストがあったらループを抜ける;
		if (IsEndRequest()) {
			break;
		}

		// 描画
		Draw();
	}

	// 終了
	Finalize();
}

void SUGER::PreDraw() {
	// ImGui内部コマンド生成
	imguiManager_->EndFrame();
	// DirectX描画前処理
	// RenderTexture描画前処理
	// レンダーターゲットをセット
	targetRenderPass_->SetRenderTarget(RenderTargetType::kRenderTexture);
	// レンダーターゲットビューをクリア
	targetRenderPass_->ClearRenderTarget(RenderTargetType::kRenderTexture);
	// 深度をクリア
	depthStencil_->ClearDepthView();
	// ビューポートの設定
	viewPort_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();
	// SrvManager描画前処理
	srvUavManager_->PreCommand();
}

void SUGER::PostDraw() {
	// DirectX描画後処理
	// スワップチェーン描画前のバリアを張る
	barrier_->PostDrawBarrierTransition();
	// レンダーターゲットを設定
	targetRenderPass_->SetRenderTarget(RenderTargetType::kSwapChain);
	// 画面をクリア
	targetRenderPass_->ClearRenderTarget(RenderTargetType::kSwapChain);
	// ビューポートの設定
	viewPort_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();

	// レンダーテクスチャの描画
	renderTexture_->Draw();

	// ImGui描画処理
	imguiManager_->Draw();

	// 描画後のバリアを張る
	barrier_->PreSwapChainPresentBarrierTransition();
	// コマンドの実行
	command_->KickCommand();
	// GPUとOSに画面の交換を行うように通知する
	swapChain_->Present();
	// GPUを待機
	fence_->WaitGPU();
	// コマンドをリセット
	command_->ResetCommand();
}

HWND SUGER::GetWindowHandle() {
	return windowManager_->GetHwnd();
}

int64_t SUGER::GetMouseWheelDelta() {
	return windowManager_->GetMouseWheelDelta();
}

bool SUGER::PushKey(BYTE keyNumber) {
	return directInput_->PushKey(keyNumber);
}

bool SUGER::TriggerKey(BYTE keyNumber) {
	return directInput_->TriggerKey(keyNumber);
}

bool SUGER::HoldKey(BYTE keyNumber) {
	return directInput_->HoldKey(keyNumber);
}

bool SUGER::ReleaseKey(BYTE keyNumber) {
	return directInput_->ReleaseKey(keyNumber);
}

bool SUGER::PushButton(int controllerID, int buttonNumber) {
	return directInput_->PushButton(controllerID, buttonNumber);
}

bool SUGER::TriggerButton(int controllerID, int buttonNumber) {
	return directInput_->TriggerButton(controllerID, buttonNumber);
}

bool SUGER::HoldButton(int controllerID, int buttonNumber) {
	return directInput_->HoldButton(controllerID, buttonNumber);
}

bool SUGER::ReleaseButton(int controllerID, int buttonNumber) {
	return directInput_->ReleaseButton(controllerID, buttonNumber);
}

int SUGER::GetLeftStickX(int controllerID) {
	return directInput_->GetLeftStickX(controllerID);
}

int SUGER::GetLeftStickY(int controllerID) {
	return directInput_->GetLeftStickY(controllerID);
}

int SUGER::GetRightStickX(int controllerID) {
	return directInput_->GetRightStickX(controllerID);
}

int SUGER::GetRightStickY(int controllerID) {
	return directInput_->GetRightStickY(controllerID);
}

int SUGER::GetLeftTrigger(int controllerID) {
	return directInput_->GetLeftTrigger(controllerID);
}

int SUGER::GetRightTrigger(int controllerID) {
	return directInput_->GetRightTrigger(controllerID);
}

bool SUGER::IsPadUp(int controllerID) {
	return directInput_->IsPadUp(controllerID);
}

bool SUGER::IsPadRight(int controllerID) {
	return directInput_->IsPadRight(controllerID);
}

bool SUGER::IsPadDown(int controllerID) {
	return directInput_->IsPadDown(controllerID);
}

bool SUGER::IsPadLeft(int controllerID) {
	return directInput_->IsPadLeft(controllerID);
}

void SUGER::InitializeFixFPS() {
	fixFPS_->Initialize();
}

ID3D12Device* SUGER::GetDirectXDevice() {
	return dxgiManager_->GetDevice();
}

ComPtr<ID3D12Resource> SUGER::CreateBufferResource(size_t sizeInBytes, bool isuav) {
	return dxgiManager_->CreateBufferResource(sizeInBytes, isuav);
}

ID3D12GraphicsCommandList* SUGER::GetDirectXCommandList() {
	return command_->GetList();
}

void SUGER::KickCommand() {
	command_->KickCommand();
}

void SUGER::ResetCommand() {
	command_->ResetCommand();
}

void SUGER::WaitGPU() {
	fence_->WaitGPU();
}

D3D12_CPU_DESCRIPTOR_HANDLE SUGER::GetRTVDescriptorHandleCPU(uint32_t index) {
	return rtvManager_->GetDescriptorHandleCPU(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE SUGER::GetRTVDescriptorHandleGPU(uint32_t index) {
	return rtvManager_->GetDescriptorHandleGPU(index);
}

uint32_t SUGER::RTVAllocate() {
	return rtvManager_->Allocate();
}

void SUGER::CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource) {
	rtvManager_->CreateRTVTexture2d(rtvIndex, pResource);
}

D3D12_CPU_DESCRIPTOR_HANDLE SUGER::GetSRVUAVDescriptorHandleCPU(uint32_t index) {
	return srvUavManager_->GetDescriptorHandleCPU(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE SUGER::GetSRVUAVDescriptorHandleGPU(uint32_t index) {
	return srvUavManager_->GetDescriptorHandleGPU(index);
}

void SUGER::SetComputeRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) {
	srvUavManager_->SetComputeRootDescriptorTable(rootParameterIndex, srvIndex);
}

void SUGER::PreCommand() {
	srvUavManager_->PreCommand();
}

void SUGER::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) {
	srvUavManager_->SetGraphicsRootDescriptorTable(rootParameterIndex, srvIndex);
}

uint32_t SUGER::ViewAllocate() {
	return srvUavManager_->Allocate();
}

void SUGER::CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	srvUavManager_->CreateSrvStructuredBuffer(viewIndex, pResource, numElements, structureByteStride);
}

void SUGER::CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	srvUavManager_->CreateUavStructuredBuffer(viewIndex, pResource, numElements, structureByteStride);
}

void SUGER::LoadTexture(const std::string& filePath) {
	textureManager_->Load(filePath);
}

std::unordered_map<std::string, Texture>& SUGER::GetTexture() {
	return textureManager_->GetTexture();
}

const DirectX::TexMetadata& SUGER::GetTextureMetaData(const std::string& filePath) {
	return textureManager_->GetMetaData(filePath);
}

ID3D12PipelineState* SUGER::GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return graphicsPipelineManager_->GetPipelineState(pipelineState, blendMode);
}

ID3D12RootSignature* SUGER::GetRootSignature(ComputePipelineStateType pipelineState) {
	return computePipelineManager_->GetRootSignature(pipelineState);
}

ID3D12PipelineState* SUGER::GetPipelineState(ComputePipelineStateType pipelineState) {
	return computePipelineManager_->GetPipelineState(pipelineState);
}

void SUGER::LoadModel(const std::string& filePath) {
	modelManager_->Load(filePath);
}

void SUGER::CreateSphere(const std::string& textureFilePath) {
	modelManager_->CreateSphere(textureFilePath);
}

Model* SUGER::FindModel(const std::string& filePath) {
	return modelManager_->Find(filePath);
}

std::string SUGER::Create2DObject(const std::string& name, const std::string& filePath) {
	return object2dManager_->Create(name, filePath);
}

void SUGER::Update2DObjects() {
	object2dManager_->Update();
}

void SUGER::Draw2DObjects() {
	object2dManager_->Draw();
}

Sprite* SUGER::FindObject2D(const std::string& name) {
	return object2dManager_->Find(name);
}

void SUGER::Clear2DObjectContainer() {
	object2dManager_->ClearContainer();
}

std::string SUGER::CreateEmpty(const std::string& name, const EulerTransform3D& transform) {
	return emptyManager_->Create(name, transform);
}

void SUGER::UpdateEmpties() {
	emptyManager_->Update();
}

Empty* SUGER::FindEmpty(const std::string& name) {
	return emptyManager_->Find(name);
}

void SUGER::ClearEmptyContainer() {
	emptyManager_->ClearContainer();
}

std::string SUGER::CreateEntity(const std::string& name, const std::string& filePath, const EulerTransform3D& transform) {
	return entityManager_->Create(name, filePath, transform);
}

void SUGER::UpdateEntities() {
	entityManager_->Update();
}

void SUGER::DrawEntiteis() {
	entityManager_->Draw();
}

void SUGER::DrawSkiningEntities() {
	entityManager_->DrawSkining();
}

Entity* SUGER::FindEntity(const std::string& name) {
	return entityManager_->Find(name);
}

void SUGER::ClearEntityContainer() {
	entityManager_->ClearContainer();
}

void SUGER::SetRequiredObjects(Camera* camera, PunctualLight* punctualLight) {
	entityManager_->SetRequiredObjects(camera, punctualLight);
	particleManager_->SetSceneCamera(camera);
	lineManager_->SetSceneCamera(camera);
}

void SUGER::SetSceneCamera(Camera* camera) {
	entityManager_->SetSceneCamera(camera);
	particleManager_->SetSceneCamera(camera);
	lineManager_->SetSceneCamera(camera);
}

std::string SUGER::CreateEmitter(const std::string& name, const EulerTransform3D& transform) {
	return emitterManager_->CreateEmitter(name, transform);
}

void SUGER::UpdateEmitters() {
	emitterManager_->Update();
}

Emitter* SUGER::FindEmitter(const std::string& name) {
	return emitterManager_->Find(name);
}

void SUGER::ClearEmitterContainer() {
	emitterManager_->ClearContainer();
}

std::string SUGER::CreateParticle(const std::string& name, const ParticleType& particleType, const std::string& filePath) {
	// 既定のディレクトリパス
	const std::string& textureDirectoryPath = "resources/images/";
	// パーティクルタイプに応じてパーティクルを作成
	switch (particleType) {
	case kPlane:
		// 板ポリパーティクルを作成
		return particleManager_->CreatePlaneParticle(name, textureDirectoryPath + filePath);
		break;
	case kModel:
		// モデルパーティクルを作成
		return particleManager_->CreateModelParticle(name, filePath);
		break;
	default:
		return "";
	}
}

void SUGER::UpdateParticles() {
	particleManager_->Update();
}

void SUGER::DrawParticles() {
	particleManager_->Draw();
}

Particle* SUGER::FindParticle(const std::string& name) {
	return particleManager_->Find(name);
}

void SUGER::ClearParticleContainer() {
	particleManager_->ClearContainer();
}

void SUGER::CreateLineGroup(const std::string& name) {
	lineManager_->CreateLine(name);
}

void SUGER::UpdateLines() {
	lineManager_->Update();
}

void SUGER::DrawLines() {
	lineManager_->Draw();
}

LineGroup* SUGER::FindLine(const std::string& name) {
	return lineManager_->Find(name);
}

void SUGER::ClearLineContainer() {
	lineManager_->ClearContainer();
}

void SUGER::LoadWaveSound(const std::string& filename, const std::string& directoryPath) {
	soundManager_->LoadWave(filename, directoryPath);
}

void SUGER::PlayWaveSound(const std::string& filename) {
	soundManager_->PlayWave(filename);
}

void SUGER::PlayWaveLoopSound(const std::string& filename, uint32_t loopCount) {
	soundManager_->PlayWaveLoop(filename, loopCount);
}

void SUGER::StopWaveSound(const std::string& filename) {
	soundManager_->StopWave(filename);
}

void SUGER::StopWaveLoopSound(const std::string& filename) {
	soundManager_->StopWaveLoop(filename);
}

void SUGER::StopWaveAllSound(const std::string& filename) {
	soundManager_->StopAll(filename);
}

void SUGER::CreanupFinishedVoices() {
	soundManager_->CleanupFinishedVoices();
}

void SUGER::AddColliderList(EntityController* entityController) {
	collisionManager_->AddCollider(entityController);
}

void SUGER::ClearColliderContainer() {
	collisionManager_->ClearContainer();
}

void SUGER::CheckAllCollisions() {
	collisionManager_->CheckAllCollisions();
}

void SUGER::InitializeColliderLineGroup() {
	collisionManager_->InitializeColliderLineGroup();
}

LineController* SUGER::GetColliderLineController() {
	return collisionManager_->GetLineController();
}

void SUGER::PreDrawObject2D() {
	object2dSystem_->PreDraw();
}

void SUGER::PreDrawObject3D() {
	object3dSystem_->PreDraw();
}

void SUGER::PreDrawObject3DSkinning() {
	object3dSystem_->PreDrawSkinning();
}

void SUGER::PreDrawParticle3D() {
	particleSystem_->PreDraw();
}

void SUGER::PreDrawLine3D() {
	lineSystem_->PreDraw();
}

void SUGER::LoadJsonLevelData(const std::string& fileName) {
	jsonLevelDataManager_->Load(fileName);
}

JsonLevelData* SUGER::FindJsonLevelData(const std::string& levelDataName) {
	return jsonLevelDataManager_->Find(levelDataName);
}

void SUGER::AddGrobalDataGroup(const std::string& groupname) {
	grobalDataManager_->CreateGroup(groupname);
}

void SUGER::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, int32_t value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void SUGER::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, float value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void SUGER::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, Vector3 value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void SUGER::AddGrobalDataItem(const std::string& groupname, const std::string& itemname, bool value) {
	grobalDataManager_->AddItem(groupname, itemname, value);
}

void SUGER::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, int32_t value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

void SUGER::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, float value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

void SUGER::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, Vector3 value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

void SUGER::SetGrobalDataValue(const std::string& groupname, const std::string& itemname, bool value) {
	grobalDataManager_->SetValue(groupname, itemname, value);
}

int32_t SUGER::GetGrobalDataValueInt(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueInt(groupName, key);
}

float SUGER::GetGrobalDataValueFloat(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueFloat(groupName, key);
}

Vector3 SUGER::GetGrobalDataValueVector3(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueVector3(groupName, key);
}

bool SUGER::GetGrobalDataValueBool(const std::string& groupName, const std::string& key) {
	return grobalDataManager_->GetValueBool(groupName, key);
}

void SUGER::ShowFPS() {
#ifdef _DEBUG
	// フレームレート表示の設定
	ImGuiIO& io = ImGui::GetIO();

	// 左上に固定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowBgAlpha(0.0f); // 背景を透明にする

	// ウィンドウを作成せずに、文字だけを描画
	ImGui::Begin("FPS Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	// 文字色を緑に変更
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // 緑色
	ImGui::Text("FPS: %.1f", io.Framerate);
	ImGui::PopStyleColor();

	ImGui::End();
#endif // _DEBUG
}
