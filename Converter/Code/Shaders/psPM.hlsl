Texture2D tex;
Texture2D normalmap;
SamplerState ss;

cbuffer LightBuffer
{
	float4 lightColor;
	float3 lightPosition;
	float ambient;
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
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};


float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = entityColor;
	float4 bumpMap = (normalmap.Sample(ss, input.tex)*2.0f) - 1.0f;
	float3 bumpNormal = normalize((bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal));
	float3 lightDirection = normalize(lightPosition - input.pos);
	float intensity = saturate(dot(bumpNormal, lightDirection));
	intensity = ambient + (1 - ambient)*intensity;
	textureColor.xyz *= lightColor.xyz*intensity;

	/*float fog = 1.0f / length(input.pos - lightPosition);
	textureColor.xyz *= (fog * 0.5f + 0.5f);*/

	return textureColor;
}