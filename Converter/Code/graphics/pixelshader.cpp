#include "pixelshader.h"
#include <fstream>

namespace gfx
{
	void PixelShader::CreatePixelShader(Graphics& graphics, ID3DBlob* shaderBuffer)
	{
		HRESULT hr = graphics.getDevice()->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
		if (FAILED(hr))
			throw std::exception("Failed to create pixel shader.");
	}
	PixelShader::PixelShader(Graphics& graphics, LPCWSTR shaderFileName)
	{
		AutoReleasePtr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);
		CreatePixelShader(graphics, shaderBuffer);
	}

	PixelShader::PixelShader(Graphics& graphics, UINT modelType)
	{
		std::string shaderCode;
		shaderCode.reserve(1024);
		shaderCode += "Texture2D tex;Texture2D normalmap;SamplerState ss;\
cbuffer LightBuffer{float4 lightColor;float3 lightPosition;float ambient;};\
cbuffer ColorBuffer{float4 entityColor;};\
struct PixelInputType{";
		if (ModelType::HasPositions(modelType))
			shaderCode += "float4 position:SV_POSITION;";
		if (ModelType::HasNormals(modelType))
			shaderCode += "float3 pos:POSITION;";
		if (ModelType::HasTexcoords(modelType))
			shaderCode += "float2 tex:TEXCOORD;";
		if (ModelType::HasNormals(modelType))
			shaderCode += "float3 normal:NORMAL;";
		if (ModelType::HasTangentsBinormals(modelType))
			shaderCode += "float3 tangent:TANGENT;float3 binormal:BINORMAL;";
		shaderCode += "};float4 main(PixelInputType input):SV_TARGET{float4 color=";
		shaderCode += ModelType::HasTexture(modelType) ? "tex.Sample(ss,input.tex);" : "entityColor;";
		if (ModelType::HasNormals(modelType))
		{
			if (ModelType::HasNormalmap(modelType))
				shaderCode += "float4 bumpMap=(normalmap.Sample(ss,input.tex)*2)-1;\
input.normal=normalize((bumpMap.x*input.tangent)+(bumpMap.y*input.binormal)+(bumpMap.z*input.normal));";
			shaderCode += "float3 lightDirection=normalize(lightPosition-input.pos);\
float intensity=saturate(dot(input.normal,lightDirection));\
intensity=ambient+(1-ambient)*intensity;\
color.xyz*=lightColor.xyz*intensity;";
		}
		shaderCode += "return color;}";

		AutoReleasePtr<ID3DBlob> shaderBuffer = CompileShader(shaderCode, "main", "ps_5_0", L"shadererror.txt");
		CreatePixelShader(graphics, shaderBuffer);
	}

	void PixelShader::SetShaderToRender(Graphics& graphics)
	{
		graphics.getContext()->PSSetShader(m_pixelShader, nullptr, 0);
	}

	void PixelShader::SetCBuffer(Graphics& graphics, CBuffer& buffer, UINT index)
	{
		ID3D11Buffer* bufferPtr = buffer.getBuffer();
		graphics.getContext()->PSSetConstantBuffers(index, 1, &bufferPtr);
	}
}