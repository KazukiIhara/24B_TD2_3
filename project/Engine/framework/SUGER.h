#pragma once
// C++
#include <string>
#include <unordered_map>
#include <memory>

#include <d3d12.h>

#include "manager/window/WindowManager.h"
#include "input/direct/DirectInput.h"
#include "FixFPS/FixFPS.h"

#include "directX/dxgi/DXGIManager.h"
#include "directX/command/DirectXCommand.h"
#include "directX/fence/Fence.h"

#include "manager/rtv/RTVManager.h"
#include "manager/dsv/DSVManager.h"
#include "manager/srvUav/SRVUAVManager.h"

#include "manager/texture/TextureManager.h"
#include "manager/model/ModelManager.h"
#include "manager/sound/SoundManager.h"

#include "manager/object/2d/Object2DManager.h"
#include "manager/emptyManager/EmptyManager.h"
#include "manager/entityManager/EntityManager.h"
#include "manager/emitter/EmitterManager.h"
#include "manager/particle/ParticleManager.h"
#include "manager/line/LineGroupManager.h"

#include "manager/pipeline/graphics/GraphicsPipelineManager.h"
#include "manager/pipeline/compute/ComputePipelineManager.h"
#include "manager/pipeline/postEffect/PostEffectPipelineManager.h"

#include "manager/data/level/json/JsonLevelDataManager.h"
#include "manager/data/grobal/GrobalDataManager.h"

#include "manager/collision/CollisionManager.h"
#include "manager/imgui/ImGuiManager.h"

#include "directX/swapChain/SwapChain.h"
#include "directX/renderTexture/RenderTexture.h"
#include "directX/depthStencil/DepthStencil.h"
#include "directX/barrier/Barrier.h"
#include "directX/targetRenderPass/TargetRenderPass.h"
#include "directX/viewport/ViewPort.h"
#include "directX/scissorRect/ScissorRect.h"

#include "system/object2d/Object2dSystem.h"
#include "system/object3d/Object3dSystem.h"
#include "system/particle/ParticleSystem.h"
#include "system/line/LineSystem.h"

#include "iScene/abstractFactory/AbstractSceneFactory.h"

#ifdef _DEBUG
#include "debugTools/leakChecker/d3dResource/D3DResourceLeakChecker.h"
#endif // _DEBUG

class SUGER {
public:
	// 仮想デストラクタ
	virtual~SUGER() = default;
	// 初期化
	virtual void Initialize();
	// 終了
	virtual void Finalize();
	// 更新
	virtual void Update();
	// 描画
	virtual void Draw();
	// 終了チェック
	virtual bool IsEndRequest() {
		return endRequest_;
	}
public: // 公開メンバ関数
	// 実行
	void Run();
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();
public: // クラスメソッド

#pragma region WindowManager
	// WindowManagerの機能
	// Windowハンドルの取得
	static HWND GetWindowHandle();
	// マウスホイールの回転量取得
	static int64_t GetMouseWheelDelta();
#pragma endregion

#pragma region DirectInput
	// DirectInputの機能
	// キーを押している
	static bool PushKey(BYTE keyNumber);
	// キーを押した
	static bool TriggerKey(BYTE keyNumber);
	// キーを押し続けている
	static bool HoldKey(BYTE keyNumber);
	// キーを離した
	static bool ReleaseKey(BYTE keyNumber);

	// ボタンを押している
	static bool PushButton(int controllerID, int buttonNumber);
	// ボタンを押した
	static bool TriggerButton(int controllerID, int buttonNumber);
	// ボタンを押し続けている
	static bool HoldButton(int controllerID, int buttonNumber);
	// ボタンを離した
	static bool ReleaseButton(int controllerID, int buttonNumber);

	// 左スティックのX軸位置を取得（-1000～1000の範囲にスケーリング）
	static int GetLeftStickX(int controllerID);
	// 左スティックのY軸位置を取得（-1000～1000の範囲にスケーリング）
	static int GetLeftStickY(int controllerID);

	// 右スティックのX軸位置を取得（-1000～1000の範囲にスケーリング）
	static int GetRightStickX(int controllerID);
	// 右スティックのY軸位置を取得（-1000～1000の範囲にスケーリング）
	static int GetRightStickY(int controllerID);

	// 左トリガーの状態を取得
	static int GetLeftTrigger(int controllerID);
	// 右トリガーの状態を取得
	static int GetRightTrigger(int controllerID);

	// 方向キーの取得
	static bool IsPadUp(int controllerID);
	static bool IsPadRight(int controllerID);
	static bool IsPadDown(int controllerID);
	static bool IsPadLeft(int controllerID);
#pragma endregion

#pragma region FixFPS
	// FixFPSの初期化
	static void InitializeFixFPS();
#pragma endregion

#pragma region DXGI
	// デバイス取得
	static ID3D12Device* GetDirectXDevice();
	// バッファリソースを作成
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes, bool isUav = false);
#pragma endregion

#pragma region Command
	// コマンドリストを取得
	static ID3D12GraphicsCommandList* GetDirectXCommandList();
	// コマンド実行
	static void KickCommand();
	// コマンドをリセット
	static void ResetCommand();
#pragma endregion

#pragma region Fence
	// GPUをまつ
	static void WaitGPU();
#pragma endregion

#pragma region RTVManager
	// CPUの特定のインデックスハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleCPU(uint32_t index);
	// GPUの特定のインデックスハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleGPU(uint32_t index);
	// 割り当て関数
	static uint32_t RTVAllocate();
	// RTVの作成
	static void CreateRTVTexture2d(uint32_t rtvIndex, ID3D12Resource* pResource);
#pragma endregion

#pragma region SRVUAVManager
	// ViewManagerの機能
	// CPUの特定のインデックスハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSRVUAVDescriptorHandleCPU(uint32_t index);
	// GPUの特定のインデックスハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSRVUAVDescriptorHandleGPU(uint32_t index);
	// 計算ディスクリプターテーブルのセット
	static void SetComputeRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);
	// コマンド前処理
	static void PreCommand();
	// ディスクリプターテーブルのセット
	static void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);
	// Allocate
	static uint32_t ViewAllocate();
	// instancing用のsrv作成
	static void CreateSrvStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
	// UAV作成
	static void CreateUavStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
#pragma endregion

#pragma region RenderTarget



#pragma endregion

#pragma region ImGuiManager
	// ImGuiManagerの機能

#pragma endregion

#pragma region TextureManager
	// TextureManagerの機能
	// 画像読み込み関数
	static void LoadTexture(const std::string& filePath);
	// 画像取得関数
	static std::unordered_map<std::string, Texture>& GetTexture();
	// メタデータ取得
	static const DirectX::TexMetadata& GetTextureMetaData(const std::string& filePath);
#pragma endregion

#pragma region GraphicsPipelineManager
	// GraphicsPipelineManagerの機能
	// パイプライン取得関数
	static ID3D12PipelineState* GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);
#pragma endregion

#pragma region ComputePipelineManager
	// ComputePipelineManagerの機能
	// ルートシグネイチャ取得関数
	static ID3D12RootSignature* GetRootSignature(ComputePipelineStateType pipelineState);
	// パイプライン取得関数
	static ID3D12PipelineState* GetPipelineState(ComputePipelineStateType pipelineState);
#pragma endregion

#pragma region PostEffectPipelineManager

#pragma endregion

#pragma region ModelManager
	// モデルの読み込み
	static void LoadModel(const std::string& filePath);
	// 球体の作成
	static void CreateSphere(const std::string& textureFilePath);
	// モデルの検索
	static Model* FindModel(const std::string& filePath);
#pragma endregion

#pragma region Object2DManager
	// 2Dオブジェクトの作成
	static std::string Create2DObject(const std::string& name, const std::string& filePath);
	// 2Dオブジェクトの更新
	static void Update2DObjects();
	// 2Dオブジェクトの描画
	static void Draw2DObjects();
	// 2Dオブジェクト検索
	static Sprite* FindObject2D(const std::string& name);
	// 2Dオブジェクトコンテナのクリア
	static void Clear2DObjectContainer();
#pragma endregion

#pragma region EmptyManager
	// エンプティの作成
	static std::string CreateEmpty(const std::string& name, const EulerTransform3D& transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
	// エンプティの更新
	static void UpdateEmpties();
	// エンプティの検索
	static Empty* FindEmpty(const std::string& name);
	// エンプティコンテナのクリア
	static void ClearEmptyContainer();
#pragma endregion

#pragma region EntityManager
	// エンティティの作成
	static std::string CreateEntity(const std::string& name, const std::string& filePath, const EulerTransform3D& transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
	// エンティティの更新
	static void UpdateEntities();
	// エンティティの描画
	static void DrawEntiteis();
	// スキニング付きエンティティの描画
	static void DrawSkiningEntities();
	// エンティティ検索
	static Entity* FindEntity(const std::string& name);
	// エンティティコンテナのクリア
	static void ClearEntityContainer();

	// シーンのカメラとライトをセット
	static void SetRequiredObjects(Camera* camera, PunctualLight* punctualLight);
	// シーンのカメラをセット
	static void SetSceneCamera(Camera* camera);
#pragma endregion

#pragma region EmitterManager
	// Emitterの作成
	static std::string CreateEmitter(const std::string& name, const EulerTransform3D& transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
	// Emitterの更新
	static void UpdateEmitters();
	// Emitterの検索
	static Emitter* FindEmitter(const std::string& name);
	// エミッターコンテナのクリア
	static void ClearEmitterContainer();
#pragma endregion

#pragma region ParticleManager
	// Particleの作成
	static std::string CreateParticle(const std::string& name, const ParticleType& particleType, const std::string& filePath);
	// Particleの更新
	static void UpdateParticles();
	// Particleの描画
	static void DrawParticles();
	// パーティクル検索
	static Particle* FindParticle(const std::string& name);
	// パーティクルコンテナのクリア
	static void ClearParticleContainer();
#pragma endregion

#pragma region LineManager
	// Lineの作成
	static void CreateLineGroup(const std::string& name);
	// Lineの更新
	static void UpdateLines();
	// Lineの描画
	static void DrawLines();
	// Line検索
	static LineGroup* FindLine(const std::string& name);
	// Lineコンテナのクリア
	static void ClearLineContainer();
#pragma endregion

#pragma region SoundManager
	// 音声読み込み
	static void LoadWaveSound(const std::string& filename, const std::string& directoryPath = "resources/sounds");
	// 音声再生
	static void PlayWaveSound(const std::string& filename);
	// 音声ループ再生
	static void PlayWaveLoopSound(const std::string& filename, uint32_t loopCount = XAUDIO2_LOOP_INFINITE);
	// 音声停止
	static void StopWaveSound(const std::string& filename);
	// 音声ループ再生停止
	static void StopWaveLoopSound(const std::string& filename);
	// 両再生停止処理
	static void StopWaveAllSound(const std::string& filename);
	// 再生が終わっているサウンドを自動的に再生中コンテナから削除
	static void CreanupFinishedVoices();
#pragma endregion

#pragma region ColliderManager
	// リストにコライダーを追加
	static void AddColliderList(EntityController* entityController);
	// コライダーの中身をリセット
	static void ClearColliderContainer();
	// コライダーリスト内の全当たり判定をチェック
	static void CheckAllCollisions();
	// デバッグ描画用のライングループとコントローラを作成
	static void InitializeColliderLineGroup();
	// デバッグ描画用のラインコントローラを取得
	static LineController* GetColliderLineController();
#pragma endregion

#pragma region JsonLevelDataManager
	// JsonLevelDataManagerの機能
	// Json形式のlevelデータの読み込み
	static void LoadJsonLevelData(const std::string& fileName);
	// Json形式のLevelDataの検索
	static JsonLevelData* FindJsonLevelData(const std::string& levelDataName);
#pragma endregion

#pragma region GrobalDataManager
	// グループの追加
	static void AddGrobalDataGroup(const std::string& groupname);
	// アイテムの追加
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, int32_t value);
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, float value);
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, Vector3 value);
	static void AddGrobalDataItem(const std::string& groupname, const std::string& itemname, bool value);

	// グループとキーから値をセット
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, int32_t value);
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, float value);
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, Vector3 value);
	static void SetGrobalDataValue(const std::string& groupname, const std::string& itemname, bool value);

	// グループとキーから値の取得
	static int32_t GetGrobalDataValueInt(const std::string& groupName, const std::string& key);
	static float GetGrobalDataValueFloat(const std::string& groupName, const std::string& key);
	static Vector3 GetGrobalDataValueVector3(const std::string& groupName, const std::string& key);
	static bool GetGrobalDataValueBool(const std::string& groupName, const std::string& key);
#pragma endregion

#pragma region Object2DSystem
	// Object2DSystemの機能
	// 2dオブジェクト描画前処理
	static void PreDrawObject2D();
#pragma endregion

#pragma region Object3DSystem
	// Object3DSystemの機能
	// 3dオブジェクト描画前処理
	static void PreDrawObject3D();
	// Skinningあり3dオブジェクト描画前処理
	static void PreDrawObject3DSkinning();

#pragma endregion

#pragma region ParticleSystem
	// ParticleSystemの機能
	// Particleの描画前処理
	static void PreDrawParticle3D();

#pragma endregion

#pragma region LineSystem
	// LineSystem
	// Lineの描画前処理
	static void PreDrawLine3D();

#pragma endregion

#pragma region ImGuiDebug
	// FPSの表示
	void ShowFPS();
#pragma endregion


private: // メンバ変数
	// 終了リクエスト
	bool endRequest_ = false;
private: // クラスのポインタ
#ifdef _DEBUG
	static std::unique_ptr<D3DResourceLeakChecker> leakCheck_;
#endif // _DEBUG
	static std::unique_ptr<WindowManager> windowManager_;
	static std::unique_ptr<DirectInput> directInput_;
	static std::unique_ptr<FixFPS> fixFPS_;

	static std::unique_ptr<DXGIManager> dxgiManager_;
	static std::unique_ptr<DirectXCommand> command_;
	static std::unique_ptr<Fence> fence_;

	static std::unique_ptr<RTVManager> rtvManager_;
	static std::unique_ptr<DSVManager> dsvmanager_;
	static std::unique_ptr<SRVUAVManager> srvUavManager_;

	static std::unique_ptr<TextureManager> textureManager_;
	static std::unique_ptr<ModelManager> modelManager_;
	static std::unique_ptr<SoundManager> soundManager_;

	static std::unique_ptr<Object2DManager> object2dManager_;
	static std::unique_ptr<EmptyManager> emptyManager_;
	static std::unique_ptr<EntityManager> entityManager_;
	static std::unique_ptr<EmitterManager> emitterManager_;
	static std::unique_ptr<ParticleManager> particleManager_;
	static std::unique_ptr<LineGroupManager> lineManager_;

	static std::unique_ptr<GraphicsPipelineManager> graphicsPipelineManager_;
	static std::unique_ptr<ComputePipelineManager> computePipelineManager_;
	static std::unique_ptr<PostEffectPipelineManager> postEffectPipelineManager_;

	static std::unique_ptr<JsonLevelDataManager> jsonLevelDataManager_;
	static std::unique_ptr<GrobalDataManager> grobalDataManager_;

	static std::unique_ptr<CollisionManager> collisionManager_;
	static std::unique_ptr<ImGuiManager> imguiManager_;


	static std::unique_ptr<SwapChain> swapChain_;
	static std::unique_ptr<RenderTexture> renderTexture_;
	static std::unique_ptr<DepthStencil> depthStencil_;
	static std::unique_ptr<Barrier> barrier_;
	static std::unique_ptr<TargetRenderPass> targetRenderPass_;
	static std::unique_ptr<ViewPort> viewPort_;
	static std::unique_ptr<ScissorRect> scissorRect_;

	static std::unique_ptr<Object2DSystem> object2dSystem_;
	static std::unique_ptr<Object3DSystem> object3dSystem_;
	static std::unique_ptr<ParticleSystem> particleSystem_;
	static std::unique_ptr<LineSystem> lineSystem_;

};