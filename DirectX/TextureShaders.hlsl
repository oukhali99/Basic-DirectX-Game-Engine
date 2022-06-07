cbuffer CBuf : register(b0) {
    matrix worldTransformation;
    matrix viewTransformation;
};

cbuffer CBuf : register(b1)
{
    float4 faceColors[5];
};

cbuffer CBuf : register(b2)
{
    struct foo
    {
        float4 lightPos;
        float4 ambient;
        float4 diffuseColor;
        float diffuseIntensity;
        float attConst;
        float attLin;
        float attQuad;
    } asd[1];
};

Texture2DArray my_texture : register(t0);
SamplerState my_sampler : register(s0);


struct VS_Out {
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORDS;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

VS_Out VShader(float3 position : POSITION, float3 normal : NORMAL, float2 texcoords : TEXCOORDS)
{
    VS_Out output;
    output.position = mul(float4(position, 1), viewTransformation);
    output.texcoords = texcoords;
    output.worldPosition = (float3)mul(float4(position, 1), worldTransformation);
    output.normal = (float3)mul(normal, (float3x3)worldTransformation);

    return output;
}


float4 PShader(VS_Out input, uint tid: SV_PrimitiveID) : SV_TARGET
{
    const float3 lightPos = (float3) asd[0].lightPos;
    const float3 ambient = (float3) asd[0].ambient;
    const float3 diffuseColor = (float3) asd[0].diffuseColor;
    
    const float3 vToL = lightPos - input.worldPosition;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    const float att = 1 / (asd[0].attConst + asd[0].attLin * distToL + asd[0].attQuad * (distToL * distToL));
    const float3 diffuse = diffuseColor * asd[0].diffuseIntensity * att * max(0, dot(dirToL, input.normal));

    float3 texCoords = { input.texcoords.x, input.texcoords.y, 0 };
    float3 textureColor = (float3)my_texture.Sample(my_sampler, texCoords, 0);
    return float4(saturate((diffuse + ambient + (float3) faceColors[tid / 2]) * textureColor), 1);
}
