#define LIGHT_COUNT 12

cbuffer CBuf : register(b0)
{
    matrix worldTransformation;
    matrix viewTransformation;
};

cbuffer CBuf : register(b1)
{
    float4 faceColors[5];
};

cbuffer CBuf : register(b2)
{
    struct LightData
    {
        float4 lightPos;
        float4 ambient;
        float4 diffuseColor;
        float diffuseIntensity;
        float attConst;
        float attLin;
        float attQuad;
    } lights[LIGHT_COUNT];
};

Texture2DArray my_texture : register(t0);
SamplerState my_sampler : register(s0);


struct VS_Out
{
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
    output.worldPosition = (float3) mul(float4(position, 1), worldTransformation);
    output.normal = (float3) mul(normal, (float3x3) worldTransformation);

    return output;
}


float4 PShader(VS_Out input, uint tid : SV_PrimitiveID) : SV_TARGET
{
    float3 lightingSum = { 0, 0, 0 };
    
    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        if (lights[i].diffuseIntensity == 0)
        {
            continue;
        }
        
        const float3 lightPos = (float3) lights[i].lightPos;
        const float3 ambient = (float3) lights[i].ambient;
        const float3 diffuseColor = (float3) lights[i].diffuseColor;
    
        const float3 vToL = lightPos - input.worldPosition;
        const float distToL = length(vToL);
        const float3 dirToL = vToL / distToL;
        const float att = 1 / (lights[i].attConst + lights[i].attLin * distToL + lights[i].attQuad * (distToL * distToL));
        const float3 diffuse = diffuseColor * lights[i].diffuseIntensity * att * max(0, dot(dirToL, input.normal));
        
        lightingSum = lightingSum + diffuse + ambient;
    }

    float3 texCoords = { input.texcoords.x, input.texcoords.y, 0 };
    float3 surfaceColor = (float3) my_texture.Sample(my_sampler, texCoords, 0) + (float3) faceColors[tid / 2];
    
    return float4(saturate(lightingSum * surfaceColor), 1);
}
