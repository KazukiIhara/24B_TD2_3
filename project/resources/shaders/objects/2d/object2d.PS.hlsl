#include "Object2D.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // Textureのaの値が0.5以下の時にPixelを廃却
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    output.color = gMaterial.color * textureColor;
    
      
    // output.colorのa値が0の時にPixelを廃却
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    output.color = pow(output.color, 2.2);
    
    return output;
}

