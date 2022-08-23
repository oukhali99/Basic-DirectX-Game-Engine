#define LIGHT_COUNT 12

cbuffer CBuf : register(b0)
{
    matrix worldTransformation;
    matrix viewTransformation;
    matrix projectionTransformation;
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
Texture2D shadowMap : register(t1);
SamplerState my_sampler : register(s0);
SamplerState pointSampler : register(s1);


struct VS_Out
{
    float4 position : SV_POSITION;
    float2 texcoords : TEXCOORDS;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float4 lpos : L_POS;
};

VS_Out VShader(float3 position : POSITION, float3 normal : NORMAL, float2 texcoords : TEXCOORDS)
{
    VS_Out output;
    output.position = float4(position, 1);
    output.position = mul(output.position, worldTransformation);
    output.position = mul(output.position, viewTransformation);
    output.position = mul(output.position, projectionTransformation);
    
    output.texcoords = texcoords;
    
    output.worldPosition = (float3) mul(float4(position, 1), worldTransformation);
    
    output.normal = (float3) mul(normal, (float3x3) worldTransformation);
    output.normal = normalize(output.normal);
    
    output.lpos = mul(float4(position, 1), mul(worldTransformation, mul(projectionTransformation, matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))));

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
    
    float4 beforeShadowMappingColor = float4(saturate(lightingSum * surfaceColor), 1);
    
    
    //re-homogenize position after interpolation
    input.lpos.xyz /= input.lpos.w;
 
    //if position is not visible to the light - dont illuminate it
    //results in hard light frustum
    if (input.lpos.x < -1.0f || input.lpos.x > 1.0f ||
        input.lpos.y < -1.0f || input.lpos.y > 1.0f ||
        input.lpos.z < 0.0f || input.lpos.z > 1.0f)
        return float4(0, 0, 0, 0);
 
    //transform clip space coords to texture space coords (-1:1 to 0:1)
    input.lpos.x = input.lpos.x / 2 + 0.5;
    input.lpos.y = input.lpos.y / -2 + 0.5;
 
    //sample shadow map - point sampler
    float shadowMapDepth = shadowMap.Sample(pointSampler, input.lpos.xy).r;
 
    //if clip space z value greater than shadow map value then pixel is in shadow
    if (shadowMapDepth < input.lpos.z)
        return float4(0, 0, 0, 0);
 
    //otherwise calculate ilumination at fragment
    float3 L = normalize(input.lpos.xyz - input.worldPosition.xyz);
    float ndotl = dot(normalize(input.normal), L);
    return float4(1, 1, 1, 1) * ndotl;
}
