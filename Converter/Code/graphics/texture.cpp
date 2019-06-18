#include "texture.h"
#include "C://Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX11tex.h"
#include <fstream>

#pragma comment (lib, "C://Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/D3Dx11.lib")

void LoadTargaFromFile(LPCWSTR filename, std::vector<unsigned char>& pixels, int& width, int& height)
{
	struct TargaHeader
	{
		unsigned char idLength;
		unsigned char colormMapType;
		unsigned char imageType;
		unsigned short colorMapOrigin;
		unsigned short colorMapLength;
		unsigned char colorMapDepth;
		unsigned short xOrigin;
		unsigned short yOrigin;
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char imageDesc;

		void Read(std::istream& stream)
		{
			stream.read((char*)& idLength, 1);
			stream.read((char*)& colormMapType, 1);
			stream.read((char*)& imageType, 1);
			stream.read((char*)& colorMapOrigin, 2);
			stream.read((char*)& colorMapLength, 2);
			stream.read((char*)& colorMapDepth, 1);
			stream.read((char*)& xOrigin, 2);
			stream.read((char*)& yOrigin, 2);
			stream.read((char*)& width, 2);
			stream.read((char*)& height, 2);
			stream.read((char*)& bpp, 1);
			stream.read((char*)& imageDesc, 1);
		}
	};

	std::ifstream infile(filename, std::ios::in | std::ios::binary);
	if (!infile.is_open())
		throw std::exception((std::string("Failed to open file: ") + ToStr(filename)).c_str());

	TargaHeader header;
	header.Read(infile);
	infile.ignore(header.idLength);

	if (header.imageType != 2 && header.imageType != 10)
		throw std::exception((std::string("Failed to load file: ") + ToStr(filename)).c_str());
	if (header.bpp != 24 && header.bpp != 32)
		throw std::exception((std::string("Failed to load file: ") + ToStr(filename)).c_str());

	int pixelByteCount = header.bpp / 8;
	width = header.width;
	height = header.height;
	pixels.resize(width * height * 4);

	unsigned char pixel[4]{ 0x00, 0x00, 0x00, 0xff };
	int pixelOffset = (width * height * 4) - (width * 4);
	if (header.imageType == 2)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				infile.read((char*)& pixel, pixelByteCount);
				pixels[pixelOffset + 0] = pixel[2];
				pixels[pixelOffset + 1] = pixel[1];
				pixels[pixelOffset + 2] = pixel[0];
				pixels[pixelOffset + 3] = pixel[3];

				pixelOffset += 4;
			}
			pixelOffset -= (width * 8);
		}
	}
	else
	{
		char packetHeader;
		int len, x = 0, y = 0;
		int n = 0;
		while (n < height * width)
		{
			infile.read(&packetHeader, 1);
			len = (packetHeader & 0x7f) + 1;
			if (packetHeader & 0x80)
			{
				infile.read((char*)& pixel, pixelByteCount);
				for (int i = 0; i < len; i++)
				{
					pixels[pixelOffset + 0] = pixel[2];
					pixels[pixelOffset + 1] = pixel[1];
					pixels[pixelOffset + 2] = pixel[0];
					pixels[pixelOffset + 3] = pixel[3];
					pixelOffset += 4;
					if (++x == width)
					{
						x = 0;
						y++;
						pixelOffset -= (width * 8);
					}
				}
			}
			else
			{
				for (int i = 0; i < len; i++)
				{
					infile.read((char*)& pixel, pixelByteCount);
					pixels[pixelOffset + 0] = pixel[2];
					pixels[pixelOffset + 1] = pixel[1];
					pixels[pixelOffset + 2] = pixel[0];
					pixels[pixelOffset + 3] = pixel[3];
					pixelOffset += 4;
					if (++x == width)
					{
						x = 0;
						y++;
						pixelOffset -= (width * 8);
					}
				}
			}
			n += len;
		}
	}
}

namespace gfx
{
	void Texture::LoadTexture(Graphics& graphics, LPCWSTR filename)
	{
		std::wstring extension = GetFileExtension(filename);
		if (extension == L"tga")
			LoadTarga(graphics, filename);
		else
		{
			HRESULT hr;
			hr = D3DX11CreateShaderResourceViewFromFile(graphics.getDevice(), filename, nullptr, nullptr, &m_shaderResourceView, nullptr);
			if (FAILED(hr))
				throw std::exception((std::string("Failed to create texture: ") + ToStr(filename)).c_str());
		}
	}
	void Texture::CreateTexture(Graphics& graphics, void* data, int width, int height)
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
	void Texture::LoadTarga(Graphics& graphics, LPCWSTR filename)
	{
		std::vector<unsigned char> pixels;
		int width, height;
		LoadTargaFromFile(filename, pixels, width, height);
		CreateTexture(graphics, pixels.data(), width, height);
	}

	Texture::Texture(Graphics& graphics, LPCWSTR filename)
	{
		LoadTexture(graphics, filename);
	}
	Texture::Texture(Graphics& graphics, void* data, int width, int height)
	{
		CreateTexture(graphics, data, width, height);
	}
	Texture::P Texture::CreateTestTexture(Graphics& graphics)
	{
		unsigned r = 0xff0000ff;
		unsigned g = 0xff00ff00;
		unsigned b = 0xffff0000;
		unsigned k = 0xff000000;
		unsigned pixels[16][16];
		memset(pixels, 0xff, sizeof(pixels));
		for (int i = 0; i < 16; i++)
		{
			pixels[0][i] = k;
			pixels[0][i] = k;
			pixels[0][i] = k;
			pixels[15][i] = k;
			pixels[15][i] = k;
			pixels[15][i] = k;
			pixels[i][0] = k;
			pixels[i][0] = k;
			pixels[i][0] = k;
			pixels[i][15] = k;
			pixels[i][15] = k;
			pixels[i][15] = k;
			pixels[1][i] = k;
			pixels[1][i] = k;
			pixels[1][i] = k;
			pixels[14][i] = k;
			pixels[14][i] = k;
			pixels[14][i] = k;
			pixels[i][1] = k;
			pixels[i][1] = k;
			pixels[i][1] = k;
			pixels[i][14] = k;
			pixels[i][14] = k;
			pixels[i][14] = k;
		}
		{
			pixels[2][2] = r;
			pixels[3][2] = r;
			pixels[4][2] = r;
			pixels[5][2] = r;
			pixels[6][2] = r;
			pixels[2][3] = r;
			pixels[2][4] = r;
			pixels[4][3] = r;
			pixels[4][4] = r;
			pixels[3][5] = r;
			pixels[5][5] = r;
			pixels[6][5] = r;
		}
		{
			pixels[6][7] = g;
			pixels[6][8] = g;
			pixels[7][6] = g;
			pixels[8][6] = g;
			pixels[9][6] = g;
			pixels[10][7] = g;
			pixels[10][8] = g;
			pixels[10][9] = g;
			pixels[9][9] = g;
		}
		{
			pixels[9][10] = b;
			pixels[9][11] = b;
			pixels[9][12] = b;
			pixels[10][10] = b;
			pixels[10][13] = b;
			pixels[11][10] = b;
			pixels[11][11] = b;
			pixels[11][12] = b;
			pixels[12][10] = b;
			pixels[12][13] = b;
			pixels[13][10] = b;
			pixels[13][11] = b;
			pixels[13][12] = b;
		}
		return std::make_shared<Texture>(graphics, pixels, 16, 16);
	}
	void Texture::SetTextureToRender(Graphics& graphics, UINT index)
	{
		graphics.getContext()->PSSetShaderResources(index, 1, &m_shaderResourceView);
	}
}