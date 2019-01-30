cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix cameraMatrix;
};

struct VertexInputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.position = mul(output.position, cameraMatrix);
	output.tex = input.tex;
	return output;
}