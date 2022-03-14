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

Texture2DArray my_texture;
SamplerState my_sampler;

float4 PShader(VS_Out input, uint tid: SV_PrimitiveID) : SV_TARGET
{
    float3 texCoords = { input.texcoords.x, input.texcoords.y, tid / 2 };
    return my_texture.Sample(my_sampler, texCoords, 0);
}
