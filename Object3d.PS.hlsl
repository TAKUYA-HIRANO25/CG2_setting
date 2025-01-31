#include "object3d.hlsli"
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
};
struct DirectiomalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
struct Camera
{
    float32_t3 worldPosition;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gsampler : register(s0);
ConstantBuffer<DirectiomalLight> gDirectiomalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
struct PixelshaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelshaderOutput main(VertexShaderOutput input)
{
    float4 transformedUV = mul(float32_t4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gsampler, transformedUV.xy);
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float32_t3 reflectLight = reflect(gDirectiomalLight.direction, normalize(input.normal));
    float RdotE = dot(reflectLight, toEye);
    float32_t3 halfVector = normalize(-gDirectiomalLight.direction + toEye);
    float NDotH = dot(normalize(input.normal), halfVector);
    float specularPow = pow(saturate(NDotH), gMaterial.shininess);
    
    PixelshaderOutput output;
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectiomalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float32_t3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gDirectiomalLight.color.rgb * cos * gDirectiomalLight.intensity;
        // 鏡面反射
        float32_t3 specular =
        gDirectiomalLight.color.rgb * gDirectiomalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;

    }
    if (textureColor.a == 0.5)
    {
        discard;
    }
    if (output.color.a == 0.0)
    {
        discard;
    }
    return output;
}
