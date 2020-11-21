
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
};

float4 PSMain(PixelInputType input) : SV_TARGET
{
    return float4(input.normal.x, input.normal.y, input.normal.z, 1.0);
}
