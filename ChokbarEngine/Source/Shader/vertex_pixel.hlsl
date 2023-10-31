cbuffer cbPerObject : register(b0)
{
	row_major float4x4 gWorldViewProj;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    uint4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;

    
	float4 color = float4(input.pos, 1.0F);
    output.pos = mul(color, gWorldViewProj);
    output.color = input.color / 255.0F;

    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}