#pragma once
#pragma once
// Include

// C++
#include <string>

// MyHedder
#include "math/function/MathFunction.h"

///
/// Common
/// 

// UVTransform
struct UVTransform {
	Vector2 scale;
	float rotateZ;
	Vector2 translate;
};


///
/// 3D
/// 

// オイラー角3DTrasform
struct EulerTransform3D {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
};

// クオータニオン角3DTransform
struct QuaternionTransform3D {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Quaternion rotate = { 0.0f,0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
};

// 3DObjectData
struct EntityData {
	EulerTransform3D transform;
	std::string objectName;
	std::string modelName;
};

// 3D頂点データ用構造体
struct VertexData3D {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

// 3Dのラインデータ
struct LineData3D {
	Vector3 start;
	Vector3 end;
	Vector4 color;
};

// 3Dライン頂点データ用構造体
struct VertexDataLine3D {
	Vector4 position;
	Vector4 color;
};

// 3D用マテリアル構造体
struct Material3D {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransformMatrix;
	float shininess;
};

// UVなし3D頂点データ用構造体
struct VertexData3DUnUV {
	Vector4 position;
	Vector3 normal;
};

// トランスフォーメーション構造体
struct TransformationMatrix {
	Matrix4x4 World;
	Matrix4x4 ViewProjection;
	Matrix4x4 WorldInverseTransepose;
};


///
/// 2D
/// 

// 2DTransform
struct Transform2D {
	Vector2 size;
	float rotate;
	Vector2 position;
};

// 頂点データ用構造体
struct VertexData2D {
	Vector4 position;
	Vector2 texcoord;
};

// 2D用のマテリアル構造体
struct Material2D {
	Vector4 color;
	Matrix4x4 uvTransformMatrix;
};
