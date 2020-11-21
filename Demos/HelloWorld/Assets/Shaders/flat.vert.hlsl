
#include "Dusk/TransformData.inc.hlsl"

struct VertexInputType
{
    float4 position : POSITION;
    float4 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
};

PixelInputType VSMain(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, MVP);
    output.normal = input.normal;

    return output;
}
