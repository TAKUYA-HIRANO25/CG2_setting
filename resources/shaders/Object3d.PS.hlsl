#include "object3d.hlsli"
struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess; 
};
struct DirectiomalLight
{
    float4 color;
    float3 direction;
    float intensity;
};
struct Camera
{
    float3 worldPosition;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<DirectiomalLight> gDirectiomalLight : register(b1);
struct PixelshaderOutput
{
    float4 color : SV_TARGET0;
};

PixelshaderOutput main(VertexShaderOutput input)
{
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    PixelshaderOutput output;
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    if (textureColor.a == 0.0)
    {
        discard;
    }
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    if (gMaterial.enableLighting != 0)
    {
        PixelshaderOutput output;
        float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
        float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectLight = reflect(gDirectiomalLight.direction, normalize(input.normal));
        float RdotE = dot(reflectLight, toEye);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess);
        if (gMaterial.enableLighting != 0)
        {
            float NdotL = dot(normalize(input.normal), -gDirectiomalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        // 拡散反射
            float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gDirectiomalLight.color.rgb * cos * gDirectiomalLight.intensity;
        // 鏡面反射
            float3 specular =
        gDirectiomalLight.color.rgb * gDirectiomalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
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
}