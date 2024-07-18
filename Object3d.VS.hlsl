#include "object3d.hlsli"
struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
};
ConstantBuffer<TransformationMatrix> gTransformationmatrix : register(b0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.texcoord = input.texcoord;
    output.position = mul(input.position, gTransformationmatrix.WVP);
    output.normal = float3(0,0,1);
   // normalize(mul(input.normal, (float32_t3x3) gTransformationmatrix.World));
    return output;
}