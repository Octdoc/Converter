#include "pixelshader.h"

namespace gfx
{
	PixelShader::PixelShader(Graphics& graphics, LPCWSTR shaderFileName)
	{
		auto device = graphics.getDevice();
		AutoReleasePtr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);
		HRESULT hr;

		hr = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
		if (FAILED(hr))
			throw std::exception("Failed to create pixel shader.");
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