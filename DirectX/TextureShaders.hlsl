cbuffer CBuf {
    matrix worldTransformation;
    matrix viewTransformation;
};
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

Texture2DArray my_texture;
SamplerState my_sampler;

static const float3 lightPos = { 0, 0, 0 };
static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 0.8f, 0.8f, 0 };
static const float diffuseIntensity = 0.1f;
static const float attConst = 0;
static const float attLin = 1;
static const float attQuad = 1;

float4 PShader(VS_Out input, uint tid: SV_PrimitiveID) : SV_TARGET
{
    const float3 vToL = lightPos - input.worldPosition;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    const float att = attConst + attLin * distToL + attQuad * (distToL * distToL);
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0, dot(dirToL, input.normal));

    float3 texCoords = { input.texcoords.x, input.texcoords.y, 0 };
    float3 textureColor = (float3)my_texture.Sample(my_sampler, texCoords, 0);
    return float4(saturate(diffuse + ambient + textureColor), 1);
}
