cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    // float4x4 gInvView;
    float4x4 gProj;
    // float4x4 gInvProj;
    float4x4 gViewProj;
    // float4x4 gInvViewProj;
    float3 gEyePosW;
    //float cbPerObjectPad1;
    //float2 gRenderTargetSize;
    //float2 gInvRenderTargetSize;
    //float gNearZ;
    //float gFarZ;
    float gTotalTime;
    float gDeltaTime;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;

    float4 posW = mul(float4(input.pos, 1.0f), gWorld);
    output.pos = mul(posW, gViewProj);
    
    output.color = input.color;
    
    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
    return input.color;
}