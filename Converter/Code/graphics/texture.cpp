#include "texture.h"
#include "C://Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX11tex.h"

#pragma comment (lib, "C://Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/D3Dx11.lib")

namespace gfx
{
	Texture::Texture(Graphics& graphics, LPCWSTR filename)
	{
		HRESULT hr;
		hr = D3DX11CreateShaderResourceViewFromFile(graphics.getDevice(), filename, nullptr, nullptr, &m_shaderResourceView, nullptr);
		if (FAILED(hr))
			throw std::exception((std::string("Failed to create texture: ") + ToStr(filename)).c_str());
	}
	void Texture::SetTextureToRender(Graphics& graphics, UINT index)
	{
		graphics.getContext()->PSSetShaderResources(index, 1, &m_shaderResourceView);
	}
}