#include "graphics.h"

namespace gfx
{
	void Graphics::InitGraphics(int width, int height)
	{
		CreateDevice();
		CreateSwapChain(width, height);
		CreateRenderTarget(width, height);
		SetViewPort(width, height);
		CreateRasterizerStates();
		CreateBlendStates();
	}
	void Graphics::CreateDevice()
	{
		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &m_device, &featureLevel, &m_context);
		if (FAILED(hr))
			throw std::exception("Failed to create device");
	}
	void Graphics::CreateSwapChain(int width, int height)
	{
		UINT msaaQuality;
		HRESULT hr;
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, 4, &msaaQuality);
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = msaaQuality - 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_hwnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		AutoReleasePtr<IDXGIDevice> dxgi_device;
		AutoReleasePtr<IDXGIAdapter> dxgi_adapter;
		AutoReleasePtr<IDXGIFactory> dxgi_factory;
		hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
		if (FAILED(hr))
			throw std::exception("Failed to query DXGI device");
		hr = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
		if (FAILED(hr))
			throw std::exception("Failed to get adapter");
		hr = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);
		if (FAILED(hr))
			throw std::exception("Failed to get factory");

		hr = dxgi_factory->CreateSwapChain(m_device, &sd, &m_swapChain);
		if (FAILED(hr))
			throw std::exception("Failed to create swap chain");
	}
	void Graphics::CreateRenderTarget(int width, int height)
	{
		UINT msaaQuality;
		AutoReleasePtr<ID3D11Texture2D> backBuffer;
		HRESULT hr;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		if (FAILED(hr))
			throw std::exception("Failed to get back buffer");
		hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
		if (FAILED(hr))
			throw std::exception("Failed to create render target");

		D3D11_TEXTURE2D_DESC dsd;
		dsd.Width = width;
		dsd.Height = height;
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.SampleDesc.Count = 4;
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, 4, &msaaQuality);
		dsd.SampleDesc.Quality = msaaQuality - 1;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags = 0;
		dsd.MiscFlags = 0;

		hr = m_device->CreateTexture2D(&dsd, nullptr, &m_depthBuffer);
		if (FAILED(hr))
			throw std::exception("Failed to create back buffer");
		hr = m_device->CreateDepthStencilView(m_depthBuffer, nullptr, &m_depthStencilView);
		if (FAILED(hr))
			throw std::exception("Failed to create depth stencil view");

		m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	}
	void Graphics::SetViewPort(int width, int height)
	{
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		m_context->RSSetViewports(1, &vp);
	}
	void Graphics::CreateRasterizerStates()
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC rd{};
		rd.AntialiasedLineEnable = true;
		rd.CullMode = D3D11_CULL_NONE;
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0.0f;
		rd.DepthClipEnable = true;
		rd.FrontCounterClockwise = false;
		rd.MultisampleEnable = true;
		rd.ScissorEnable = false;
		rd.SlopeScaledDepthBias = 0.0f;

		rd.FillMode = D3D11_FILL_SOLID;
		hr = m_device->CreateRasterizerState(&rd, &m_rasterizerSolid);
		if (FAILED(hr))
			throw std::exception("Failed to create rasterizer state");

		rd.FillMode = D3D11_FILL_WIREFRAME;
		hr = m_device->CreateRasterizerState(&rd, &m_rasterizerWireframe);
		if (FAILED(hr))
			throw std::exception("Failed to create rasterizer state");

		m_context->RSSetState(m_rasterizerSolid);
	}
	void Graphics::CreateBlendStates()
	{
		HRESULT result;
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.RenderTarget[0].BlendEnable = false;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		blendDesc.RenderTarget[0].BlendEnable = true;
		result = m_device->CreateBlendState(&blendDesc, &m_blendState_alphaOn);
		if (FAILED(result))
			throw std::exception("Failed to create blend state");

		blendDesc.RenderTarget[0].BlendEnable = false;
		result = m_device->CreateBlendState(&blendDesc, &m_blendState_alphaOff);
		if (FAILED(result))
			throw std::exception("Failed to create blend state");

		EnableAlphaBlending(true);
	}
	Graphics::Graphics(HWND hwnd, int width, int height) :
		m_hwnd(hwnd)
	{
		InitGraphics(width, height);
	}
	void Graphics::RasterizerWireframe()
	{
		m_context->RSSetState(m_rasterizerWireframe);
	}
	void Graphics::RasterizerSolid()
	{
		m_context->RSSetState(m_rasterizerSolid);
	}
	void Graphics::EnableAlphaBlending(bool blend)
	{
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_context->OMSetBlendState(blend ? m_blendState_alphaOn : m_blendState_alphaOff, blendFactor, 0xffffffff);
	}
	void Graphics::ClearScreen()
	{
		ClearScreen(0.0f, 0.0f, 0.0f);
	}
	void Graphics::ClearScreen(float r, float g, float b, float a)
	{
		float clearColor[] = { r, g, b, a };
		m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
		m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	void Graphics::Present()
	{
		m_swapChain->Present(0, 0);
	}
}