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

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;

    float4 posW = mul(float4(input.pos, 1.0f), gWorld);
    output.pos = mul(posW, gViewProj);
        
    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}