Texture2D tex;
Texture2D normalmap;
SamplerState ss;

cbuffer LightBuffer
{
	float4 lightColor;
	float3 lightPosition;
	float lightAmbient;
};

cbuffer ColorBuffer
{
	float4 entityColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return entityColor;
}