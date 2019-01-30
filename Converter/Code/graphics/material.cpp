#include "material.h"

namespace gfx
{
	Material::Material(VertexShader::P vertexShader, PixelShader::P pixelShader, Texture::P texture, Texture::P normalmap) :
		m_vertexShader(vertexShader),
		m_pixelShader(pixelShader),
		m_texture(texture),
		m_normalmap(normalmap) {}
	void Material::SetMaterialToRender(Graphics& graphics)
	{
		m_vertexShader->SetShaderToRender(graphics);
		m_pixelShader->SetShaderToRender(graphics);
		if (m_texture)
			m_texture->SetTextureToRender(graphics, 0);
		if (m_normalmap)
			m_normalmap->SetTextureToRender(graphics, 1);
	}
}
