Texture2D gDiffuseMap : register(t0);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float gUVOffsetY;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewProj;
    float4 gLightColor;
    float3 gEyePosW;
    float gTotalTime;
    float3 gLightDirection;
    float gDeltaTime;
}

SamplerState gsamPointWrap : register(s0);
// Not used
// SamplerState gsamPointClamp         : register(s1);
// SamplerState gsamLinearWrap         : register(s2);
// SamplerState gsamLinearClamp        : register(s3);
// SamplerState gsamAnisotropicWrap    : register(s4);
// SamplerState gsamAnisotropicClamp   : register(s5);

struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 uv : TEXCOORD;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;

    float4 posW = mul(float4(input.Pos, 1.0f), gWorld);
    output.PosW = posW.xyz;
    output.PosH = mul(posW, gViewProj);
    
    output.NormalW = mul(input.Normal, (float3x3) gWorld);

    output.uv = input.uv;

    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    input.uv = input.uv + float2(0.0f, gUVOffsetY);
    
    float4 diffuse = gDiffuseMap.Sample(gsamPointWrap, input.uv);
    
    float blackThreshold = 0.01f;

    if (diffuse.r < blackThreshold && diffuse.g < blackThreshold && diffuse.b < blackThreshold)
    {
        discard;
    }
    
    return diffuse;
}