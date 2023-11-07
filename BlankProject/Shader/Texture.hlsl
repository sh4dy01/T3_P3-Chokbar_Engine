struct LightData
{
    float3 Color;
    float3 Dir;
};

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
    
    float4 gColLight;
    
    float3 gEyePosW;
    float gTotalTime;
    float3 gDirLight;
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
    float4 diffuse = gDiffuseMap.Sample(gsamPointWrap, input.uv);
    
    float3 N = normalize(input.NormalW);
    float3 L = normalize(gDirLight);
    
    // Use max() to prevent total darnkess
    float3 incomingLight = max(saturate(dot(N, L)), 0.05f) * gColLight.rgb;
    float3 finalColor = diffuse.rgb * incomingLight;
    
    return float4(finalColor, 1.0f);
}