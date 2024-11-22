#include "Particle.hlsli"

struct ParticleForGPU
{
    float4x4 WVP;
    float4x4 World;
    float4 colpr;
};
StructuredBuffer<ParticleForGPU> gParticle : register(t0);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    
};

VertexShaderOutput main(VertexShaderInput input,uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.texcoord = input.texcoord;
    output.position = mul(input.position, gParticle[instanceId].WVP);
    output.normal = normalize(mul(input.normal, (float3x3) gParticle[instanceId].World));
    output.color = gParticle[instanceId]. color;
    return output;
    
}