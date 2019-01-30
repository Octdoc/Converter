#pragma once

#include "vertexshader.h"
#include "pixelshader.h"
#include "texture.h"
#include "constantbuffer.h"

namespace gfx
{
	class Material
	{
		SMART_PTR(Material)

	private:
		VertexShader::P m_vertexShader;
		PixelShader::P m_pixelShader;
		Texture::P m_texture;
		Texture::P m_normalmap;

	public:
		Material(VertexShader::P vertexShader, PixelShader::P pixelShader, Texture::P texture = nullptr, Texture::P normalmap = nullptr);

		void SetMaterialToRender(Graphics& graphics);
	};
}