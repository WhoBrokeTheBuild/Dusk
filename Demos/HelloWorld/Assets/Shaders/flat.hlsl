
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

PixelInputType FlatVertexShader(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, TransformData.MVP);
    output.normal = input.normal;

    return output;
}

float4 FlatPixelShader(PixelInputType input) : SV_TARGET
{
    return float4(input.normal.x, input.normal.y, input.normal.z, 1.0);
}
