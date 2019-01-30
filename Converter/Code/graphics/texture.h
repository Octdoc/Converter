#pragma once

#include "graphics.h"

namespace gfx
{
	class Texture
	{
		SMART_PTR(Texture)
		NO_COPY(Texture)

	private:
		AutoReleasePtr<ID3D11ShaderResourceView> m_shaderResourceView;

	public:
		Texture(Graphics& graphics, LPCWSTR filename);

		void SetTextureToRender(Graphics& graphics, UINT index = 0);
	};
}