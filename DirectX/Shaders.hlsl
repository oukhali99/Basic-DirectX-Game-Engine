cbuffer CBuf {
    matrix transform;
};
struct VS_Out {
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORDS;
};

VS_Out VShader(float4 position : POSITION, float2 texcoords : TEXCOORDS)
{
    VS_Out output;
    output.position = mul(position, transform);
    output.texcoords = texcoords;

    return output;
}


cbuffer CBuf {
    float4 faceColors[6];
};

Texture2D my_texture;
SamplerState my_sampler;

float4 PShader(VS_Out input, uint tid: SV_PrimitiveID) : SV_TARGET
{
    return my_texture.Sample(my_sampler, input.texcoords);
}
