#pragma once

#include "shaderbase.h"
#include "constantbuffer.h"

namespace gfx
{
	class PixelShader
	{
		SMART_PTR(PixelShader)
		NO_COPY(PixelShader)

	private:
		AutoReleasePtr<ID3D11PixelShader> m_pixelShader;

	private:
		void CreatePixelShader(Graphics& graphics, ID3DBlob* shaderBuffer);

	public:
		PixelShader(Graphics& graphics, LPCWSTR shaderFileName);
		PixelShader(Graphics& graphics, UINT modelType);

		void SetShaderToRender(Graphics& graphics);
		static void SetCBuffer(Graphics& graphics, CBuffer& buffer, UINT index = 0);
	};
}