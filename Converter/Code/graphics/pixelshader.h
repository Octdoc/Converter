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

	public:
		PixelShader(Graphics& graphics, LPCWSTR shaderFileName);

		void SetShaderToRender(Graphics& graphics);
		static void SetCBuffer(Graphics& graphics, CBuffer& buffer, UINT index = 0);
	};
}