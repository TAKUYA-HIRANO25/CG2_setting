struct Material
{
    float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);
struct PixelshaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelshaderOutput main()
{
    PixelshaderOutput output;
    output.color = gMaterial.color;
    return output;
}