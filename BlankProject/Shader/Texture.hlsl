cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewProj;
    float3 gEyePosW;
    float gTotalTime;
    float gDeltaTime;
}

Texture2D gDiffuseMap : register(t0);

SamplerState gsamPointWrap          : register(s0);
// Not used
// SamplerState gsamPointClamp         : register(s1);
// SamplerState gsamLinearWrap         : register(s2);
// SamplerState gsamLinearClamp        : register(s3);
// SamplerState gsamAnisotropicWrap    : register(s4);
// SamplerState gsamAnisotropicClamp   : register(s5);

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 TextCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float2 TextCoord : TEXCOORD;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;

    float4 posW = mul(float4(input.Pos, 1.0f), gWorld);
    output.PosW = posW.xyz;
    output.PosH = mul(posW, gViewProj);
        
    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    float4 diffuse = gDiffuseMap.Sample(gsamPointWrap, input.TextCoord);
    
    return diffuse;
}