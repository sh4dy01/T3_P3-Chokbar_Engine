Texture2D gDiffuseMap : register(t0);

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

SamplerState gsamPointWrap : register(s0);

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
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
    //return float4(1, 0, 0, 1);
    float4 diffuse = gDiffuseMap.Sample(gsamPointWrap, input.uv);
    return diffuse;

}