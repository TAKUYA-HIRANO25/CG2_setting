#include "object3d.hlsli"
struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
};
struct DirectiomalLight
{
    float4 color;
    float3 direction;
    float intensity;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gsampler : register(s0);
ConstantBuffer<DirectiomalLight> gDirectiomalLight : register(b1);
struct PixelshaderOutput
{
    float4 color : SV_TARGET0;
};

PixelshaderOutput main(VertexShaderOutput input)
{
    float4 transformedUV = mul(float4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gsampler, transformedUV.xy);
    PixelshaderOutput output;
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectiomalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color = gMaterial.color * textureColor * gDirectiomalLight.color * cos * gDirectiomalLight.intensity;
    }
    else
    {
        output.color = gMaterial.color * textureColor;

    }
    return output;
}
