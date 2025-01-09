#pragma once
//  C++
#include <string>
#include <vector>
#include <map>
#include <array>
#include <span>
#include <optional>

// DirectX
#include <d3d12.h>

// MyHedder
#include "Structs/ObjectStructs.h"

// ノード
struct Node {
	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

// マテリアルデータ
struct MaterialData {
	std::string textureFilePath;
	Matrix4x4 uvMatrix;
	Vector4 color;
	bool haveUV_;
};

// GPUに送るマテリアルデータ
struct MaterialForGPU {
	Vector4 color;
	Matrix4x4 uvMatrix;
};

// メッシュデータ
struct MeshData {
	std::vector<VertexData3D> vertices;
	std::vector<uint32_t> indices;
	std::vector<VertexData3DUnUV> verticesUnUV;
	MaterialData material;
};

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

// モデルデータ
struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	Node rootNode;
	std::vector<MeshData> meshes;
};

// Vector3のアニメーションキーフレーム
struct KeyframeVector3 {
	Vector3 value;
	float time;
};

// Quaternionのアニメーションキーフレーム
struct KeyframeQuaternion {
	Quaternion value;
	float time;
};

// Nodeアニメーション
struct NodeAnimation {
	std::vector<KeyframeVector3> translate;
	std::vector<KeyframeQuaternion> rotate;
	std::vector<KeyframeVector3> scale;
};

// アニメーション
struct Animation {
	float duration;
	std::map<std::string, NodeAnimation> nodeAnimations;
};

// ジョイント
struct Joint {
	QuaternionTransform3D transform;
	Matrix4x4 loclMatrix;
	Matrix4x4 skeletonSpaceMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};

// スケルトン
struct Skeleton {
	int32_t root;
	std::map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};

// 影響を受ける頂点の数
const uint32_t kNumMaxInfluence = 4;
// 影響を受ける頂点
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointindices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};

struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResources;
	std::span<WellForGPU> mappedPalette;
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	std::span<VertexInfluence> mappedInfluence;
	uint32_t paletteSrvIndex;
	uint32_t influenceSrvIndex;
};

struct SkinningInformationForGPU {
	uint32_t numVertices;
};