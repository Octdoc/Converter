#pragma once

#include "helpers.h"

namespace gfx
{
	class Graphics
	{
		SMART_PTR(Graphics)
		NO_COPY(Graphics)

	private:
		HWND m_hwnd;

		AutoReleasePtr<ID3D11Device> m_device;
		AutoReleasePtr<ID3D11DeviceContext> m_context;
		AutoReleasePtr<IDXGISwapChain> m_swapChain;
		AutoReleasePtr<ID3D11RenderTargetView> m_renderTargetView;
		AutoReleasePtr<ID3D11Texture2D> m_depthBuffer;
		AutoReleasePtr<ID3D11DepthStencilView> m_depthStencilView;
		AutoReleasePtr<ID3D11RasterizerState> m_rasterizerWireframe;
		AutoReleasePtr<ID3D11RasterizerState> m_rasterizerSolid;
		AutoReleasePtr<ID3D11BlendState> m_blendState_alphaOn;
		AutoReleasePtr<ID3D11BlendState> m_blendState_alphaOff;

		std::function<void(float)> m_updateFunction;
		std::function<void(UINT, WPARAM, LPARAM)> m_messageFunction;

	private:
		void InitGraphics(int width, int height);

		void CreateDevice();
		void CreateSwapChain(int width, int height);
		void CreateRenderTarget(int width, int height);
		void SetViewPort(int width, int height);
		void CreateRasterizerStates();
		void CreateBlendStates();

	public:
		Graphics(HWND hwnd, int width, int height);

		void RasterizerWireframe();
		void RasterizerSolid();
		void EnableAlphaBlending(bool blend);
		void ClearScreen();
		void ClearScreen(float r, float g, float b, float a = 1.0f);
		void Present();

		inline ID3D11Device* getDevice() { return m_device; }
		inline ID3D11DeviceContext* getContext() { return m_context; }
		inline HWND getHWND() { return m_hwnd; }
	};
}