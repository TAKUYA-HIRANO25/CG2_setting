#include "object3d.hlsli"
struct Material
{
    float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gsampler : register(s0);
struct PixelshaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelshaderOutput main(VertexShaderOutput input)
{
    
    float32_t4 textureColor = gTexture.Sample(gsampler, input.texcoord);
    PixelshaderOutput output;
    output.color = gMaterial.color * textureColor;
    
    return output;
}
