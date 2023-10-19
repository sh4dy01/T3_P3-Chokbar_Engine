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

	output.pos = float4(input.pos, 1.0F);
	output.pos = mul(output.pos, gWorldViewProj);
    output.color = input.color / 255.0F;

    return output;
}

float4 ps_main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}