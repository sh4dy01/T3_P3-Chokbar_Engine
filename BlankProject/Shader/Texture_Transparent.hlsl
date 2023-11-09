Texture2D gDiffuseMap : register(t0);

cbuffer cbOffSet : register(b0)
{
    float4x4 gWorld;
    float gUVOffsetY;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gOrthoProj;
    float4x4 gViewProj;
    
    float4 gLightColor;
    float3 gEyePosW;
    float gTotalTime;
    float3 gLightDirection;
    float gDeltaTime;
}

SamplerState gsamPointWrap : register(s0);

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float2 uv : TEXCOORD;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT vOut;
    float4 pos = float4(input.Pos * 50.0f, 1.f);
    //pos = mul(float4(pos.xy, 0.0f, 1.0f), gWorld);
    pos = mul(float4(pos.xy, 0.0f, 1.0f), gOrthoProj);
    pos.x += gWorld._41;
    pos.y += gWorld._42;
    vOut.PosH = float4(pos.xy, 0.0f, 1.0f);
    vOut.uv = input.uv;

    return vOut;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    input.uv = input.uv + float2(0.0f, gUVOffsetY);
    //return float4(input.uv.x, input.uv.y, 0.0f, 1.0f);
    float4 diffuse = gDiffuseMap.Sample(gsamPointWrap, input.uv);
    
    float blackThreshold = 0.01f;

    if (diffuse.r < blackThreshold && diffuse.g < blackThreshold && diffuse.b < blackThreshold)
    {
        discard;
    }
    
    return diffuse;
}