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
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float3 lightDirection = normalize(lightPosition - input.pos);
	float intensity = saturate(dot(input.normal, lightDirection));
	intensity = lightAmbient + (1 - lightAmbient)*intensity;
	float4 color = tex.Sample(ss, input.tex);
	color.xyz *= lightColor.xyz*intensity;
	return color;
}