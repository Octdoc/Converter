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

	private:
		void LoadTexture(Graphics& graphics, LPCWSTR filename);
		void CreateTexture(Graphics& graphics, void* data, int width, int height);
		void LoadTarga(Graphics& graphics, LPCWSTR filename);

	public:
		Texture(Graphics& graphics, LPCWSTR filename);
		Texture(Graphics& graphics, void* data, int width, int height);
		static Texture::P CreateTestTexture(Graphics& graphics);

		void SetTextureToRender(Graphics& graphics, UINT index = 0);
	};
}