#include "samplerstate.h"

namespace gfx
{
	SamplerState::SamplerState(Graphics& graphics, bool interpolate)
	{
		HRESULT hr;
		D3D11_SAMPLER_DESC sd;
		sd.Filter = interpolate ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias = 0.0f;
		sd.MaxAnisotropy = 1;
		sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sd.BorderColor[0] = 0;
		sd.BorderColor[1] = 0;
		sd.BorderColor[2] = 0;
		sd.BorderColor[3] = 0;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;

		hr = graphics.getDevice()->CreateSamplerState(&sd, &m_samplerState);
		if (FAILED(hr))
			throw std::exception("Failed to create sampler state");
	}
	void SamplerState::SetSamplerState(Graphics& graphics, UINT index)
	{
		graphics.getContext()->PSSetSamplers(index, 1, &m_samplerState);
	}
}