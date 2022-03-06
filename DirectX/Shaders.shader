cbuffer CBuf {
    matrix transform;
};

float4 VShader(float4 position : POSITION) : SV_POSITION
{
    return mul(position, transform);
}


cbuffer CBuf {
    float4 faceColors[12];
};

float4 PShader(uint tid: SV_PrimitiveID) : SV_TARGET
{
    return faceColors[tid / 2];
}
