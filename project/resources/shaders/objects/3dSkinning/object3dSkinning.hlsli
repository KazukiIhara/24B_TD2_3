struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct TransformationMatrix
{
    float4x4 World;
    float4x4 ViewProjection;
    float4x4 WorldInverseTranspose;
};

struct Material
{
    float4 color;
    int enbleLighting;
    float4x4 uvTransform;
    float shininess;
};

struct ModelMaterial
{
    float4 color;
    float4x4 uvTransform;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    float cosFalloffStart;
};

struct Camera
{
    float3 worldPosition;
};

struct PunctualLight
{
    DirectionalLight directionalLight;
    PointLight pointLight;
    SpotLight spotLight;
};

struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};

struct Skinned
{
    float4 position;
    float3 normal;
};