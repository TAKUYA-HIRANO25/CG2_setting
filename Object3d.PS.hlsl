#include "object3d.hlsli"
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
};
struct DirectiomalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gsampler : register(s0);
ConstantBuffer<DirectiomalLight> gDirectiomalLight : register(b1);
struct PixelshaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelshaderOutput main(VertexShaderOutput input)
{
    
    float32_t4 textureColor = gTexture.Sample(gsampler, input.texcoord);
    PixelshaderOutput output;
    if (gMaterial.enableLighting != 0)
    {
        float cos = saturate(dot(normalize(input.normal), -gDirectiomalLight.direction));
        output.color = gMaterial.color * textureColor * gDirectiomalLight.color * cos * gDirectiomalLight.intensity;
    }
    else
    {
        output.color = gMaterial.color * textureColor;

    }
    return output;
}
