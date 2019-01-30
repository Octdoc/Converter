#include "vertexshader.h"

namespace gfx
{
	VertexShader::VertexShader(Graphics& graphics, LPCWSTR shaderFileName, UINT vertexLayout) :
		m_vertexLayout(vertexLayout)
	{
		auto device = graphics.getDevice();
		AutoReleasePtr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName); 
		HRESULT hr;
		int counter = 0;
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[7];

		hr = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
		if (FAILED(hr))
			throw std::exception("Failed to create vertex shader.");
		
		if (VertexLayout::HasPositions(m_vertexLayout))
			inputLayoutDesc[counter++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		if (VertexLayout::HasTexcoords(m_vertexLayout))
			inputLayoutDesc[counter++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		if (VertexLayout::HasNormals(m_vertexLayout))
			inputLayoutDesc[counter++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		if (VertexLayout::HasTangentsBinormals(m_vertexLayout))
		{
			inputLayoutDesc[counter++] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			inputLayoutDesc[counter++] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		if (VertexLayout::HasBones(m_vertexLayout))
		{
			inputLayoutDesc[counter++] = { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			inputLayoutDesc[counter++] = { "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		hr = device->CreateInputLayout(inputLayoutDesc, counter,
			shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &m_inputLayout);
		if (FAILED(hr))
			throw std::exception("Failed to create input layout.");
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