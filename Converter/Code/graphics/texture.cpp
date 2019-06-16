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
	Texture::Texture(Graphics& graphics, void* data, int width, int height)
	{
		ID3D11Device* device = graphics.getDevice();
		ID3D11DeviceContext* context = graphics.getContext();
		HRESULT hr;
		AutoReleasePtr<ID3D11Texture2D> texture;
		D3D11_TEXTURE2D_DESC t2dd{};
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		D3D11_SUBRESOURCE_DATA srd{};

		t2dd.Width = width;
		t2dd.Height = height;
		t2dd.MipLevels = 1;
		t2dd.ArraySize = 1;
		t2dd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		t2dd.SampleDesc.Count = 1;
		t2dd.SampleDesc.Quality = 0;
		t2dd.Usage = D3D11_USAGE_IMMUTABLE;
		t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		t2dd.CPUAccessFlags = 0;
		t2dd.MiscFlags = 0;

		srd.pSysMem = data;
		srd.SysMemPitch = width * 4;
		srd.SysMemSlicePitch = 0;
		hr = device->CreateTexture2D(&t2dd, &srd, &texture);
		if (FAILED(hr))
		{
			auto error = GetLastError();
			throw std::exception("Failed to create texture");
		}

		srvd.Format = t2dd.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = -1;

		hr = device->CreateShaderResourceView(texture, &srvd, &m_shaderResourceView);
		if (FAILED(hr))
			throw std::exception("Failed to create shader resource view");
	}
	void Texture::SetTextureToRender(Graphics& graphics, UINT index)
	{
		graphics.getContext()->PSSetShaderResources(index, 1, &m_shaderResourceView);
	}
}