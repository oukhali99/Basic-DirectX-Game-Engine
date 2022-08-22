cbuffer CBuf : register(b0)
{
    matrix worldTransformation;
    matrix viewTransformation;
    matrix projectionTransformation;
};

struct VS_Out
{
    float4 pos : SV_POSITION;
};
 
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Out VShader(float3 position : POSITION, float3 normal : NORMAL, float2 texcoords : TEXCOORDS)
{
    VS_Out output;
    output.pos = mul(float4(position, 1), mul(worldTransformation, mul(projectionTransformation, matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))));
    return output;
}
 
//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------
void PShader(VS_Out input) {}
