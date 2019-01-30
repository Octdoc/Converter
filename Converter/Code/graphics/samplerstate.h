#pragma once

#include "graphics.h"

namespace gfx
{
	class SamplerState
	{
		SMART_PTR(SamplerState)
		NO_COPY(SamplerState)

	private:
		AutoReleasePtr<ID3D11SamplerState> m_samplerState;

	public:
		SamplerState(Graphics& graphics, bool interpolate = true);

		void SetSamplerState(Graphics& graphics, UINT index = 0);
	};
}