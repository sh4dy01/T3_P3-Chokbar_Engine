struct LightData
{
    float3 Color;
    float3 Dir;
};

TextureCube gCubeMap : register(t0);

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

SamplerState gsamPointWrap  : register(s0);
SamplerState gsamLinearWrap : register(s1);

struct VS_INPUT
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT vout;

	// Use local vertex position as cubemap lookup vector.
    vout.PosL = input.PosL;
	
	// Transform to world space.
    float4 posW = mul(float4(input.PosL, 1.0f), gWorld);

	// Always center sky about camera.
    posW.xyz += gEyePosW;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosH = mul(posW, gViewProj).xyww;
	
    return vout;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    return gCubeMap.Sample(gsamLinearWrap, input.PosL);

}