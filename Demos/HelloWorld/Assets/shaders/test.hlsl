
float4 VertexShaderFunction(float4 in_Position : POSITION) : POSITION
{
    return in_Position;
}

float4 PixelShaderFunction() : COLOR0
{
    return float4(1.0, 0.0, 0.0, 1.0);
}