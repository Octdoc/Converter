cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix cameraMatrix;
};

struct VertexInputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.pos = output.position.xyz;
	output.position = mul(output.position, cameraMatrix);
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.tex = input.tex;
	return output;
}