#include "Particle.hlsli"
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
};
StructuredBuffer<TransformationMatrix> gTransformationmatrix : register(t0);

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
    output.position = mul(input.position, gTransformationmatrix[instanceId].WVP);
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationmatrix[instanceId].World));
    return output;
    
}