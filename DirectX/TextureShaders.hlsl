cbuffer CBuf {
    matrix transform;
};
struct VS_Out {
    float4 position : SV_POSITION;
    float3 texcoords : TEXCOORDS;
};

VS_Out VShader(float4 position : POSITION, float3 texcoords : TEXCOORDS)
{
    VS_Out output;
    output.position = mul(position, transform);
    output.texcoords = texcoords;

    return output;
}

Texture2D my_texture;
SamplerState my_sampler;

float4 PShader(VS_Out input, uint tid: SV_PrimitiveID) : SV_TARGET
{
    float2 texCoords = { input.texcoords.x, input.texcoords.y };
    return my_texture.Sample(my_sampler, texCoords);
}
