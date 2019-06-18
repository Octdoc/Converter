#include "vertexshader.h"
#include <fstream>

namespace gfx
{
	void VertexShader::CreateVertexShader(Graphics& graphics, ID3DBlob* shaderBuffer)
	{
		HRESULT hr = graphics.getDevice()->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
		if (FAILED(hr))
			throw std::exception("Failed to create vertex shader.");
	}
	void VertexShader::CreateInputLayout(Graphics& graphics, ID3DBlob* shaderBuffer)
	{
		int counter = 0;
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[7];
		if (ModelType::HasPositions(m_modelType))
			inputLayoutDesc[counter++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		if (ModelType::HasTexcoords(m_modelType))
			inputLayoutDesc[counter++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		if (ModelType::HasNormals(m_modelType))
			inputLayoutDesc[counter++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		if (ModelType::HasTangentsBinormals(m_modelType))
		{
			inputLayoutDesc[counter++] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			inputLayoutDesc[counter++] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		if (ModelType::HasBones(m_modelType))
		{
			inputLayoutDesc[counter++] = { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			inputLayoutDesc[counter++] = { "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		HRESULT hr = graphics.getDevice()->CreateInputLayout(inputLayoutDesc, counter,
			shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &m_inputLayout);
		if (FAILED(hr))
			throw std::exception("Failed to create input layout.");
	}
	VertexShader::VertexShader(Graphics& graphics, LPCWSTR shaderFileName, UINT modelType) :
		m_modelType(modelType)
	{
		AutoReleasePtr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);

		CreateVertexShader(graphics, shaderBuffer);
		CreateInputLayout(graphics, shaderBuffer);
	}

	VertexShader::VertexShader(Graphics& graphics, UINT modelType) :
		m_modelType(modelType)
	{
		std::string shaderCode;
		shaderCode.reserve(1024);
		shaderCode += "cbuffer MatrixBuffer{matrix worldMatrix;matrix cameraMatrix;};struct VertexInputType{";
		if (ModelType::HasPositions(modelType))
			shaderCode += "float3 position:POSITION;";
		if (ModelType::HasTexcoords(modelType))
			shaderCode += "float2 tex:TEXCOORD;";
		if (ModelType::HasNormals(modelType))
			shaderCode += "float3 normal:NORMAL;";
		if (ModelType::HasTangentsBinormals(modelType))
			shaderCode += "float3 tangent:TANGENT;float3 binormal:BINORMAL;";
		shaderCode += "};struct PixelInputType{";
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
		shaderCode += "};PixelInputType main(VertexInputType input){PixelInputType output;";
		if (ModelType::HasPositions(modelType))
			shaderCode += "output.position=mul(float4(input.position,1),worldMatrix);";
		if (ModelType::HasNormals(modelType))
			shaderCode += "output.pos=output.position.xyz;";
		if (ModelType::HasPositions(modelType))
			shaderCode += "output.position=mul(output.position,cameraMatrix);";
		if (ModelType::HasTexcoords(modelType))
			shaderCode += "output.tex=input.tex;";
		if (ModelType::HasNormals(modelType))
			shaderCode += "output.normal=mul(input.normal,(float3x3)worldMatrix);";
		if (ModelType::HasTangentsBinormals(modelType))
			shaderCode += "output.tangent=mul(input.tangent,(float3x3)worldMatrix);output.binormal=mul(input.binormal,(float3x3)worldMatrix);";
		shaderCode += "return output;}";

		AutoReleasePtr<ID3DBlob> shaderBuffer = CompileShader(shaderCode, "main", "vs_5_0", L"shadererror.txt");
		CreateVertexShader(graphics, shaderBuffer);
		CreateInputLayout(graphics, shaderBuffer);
	}

	void VertexShader::SetShaderToRender(Graphics& graphics)
	{
		auto context = graphics.getContext();
		context->IASetInputLayout(m_inputLayout);
		context->VSSetShader(m_vertexShader, nullptr, 0);
	}

	void VertexShader::SetCBuffer(Graphics& graphics, CBuffer& buffer, UINT index)
	{
		ID3D11Buffer* bufferPtr = buffer.getBuffer();
		graphics.getContext()->VSSetConstantBuffers(index, 1, &bufferPtr);
	}
}