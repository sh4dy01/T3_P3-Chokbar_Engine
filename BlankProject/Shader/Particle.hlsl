struct InstanceData
{
    float4x4 World;
    float AgeRatio;
};

StructuredBuffer<InstanceData> gInstanceData : register(t0, space1);

cbuffer cbPass : register(b0)
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
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT vs_main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
    PS_INPUT output;
    
    InstanceData instance = gInstanceData[instanceID];
    float4x4 gWorld = instance.World;

    float4 posW = mul(float4(input.pos, 1.0f), gWorld);
    output.pos = mul(posW, gViewProj);
    
    float4 color = float4(input.color.x, input.color.y, input.color.z, instance.AgeRatio);
    output.color = color;
    
    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}