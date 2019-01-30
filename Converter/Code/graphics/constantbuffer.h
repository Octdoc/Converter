#pragma once

#include "graphics.h"

namespace gfx
{
	class CBuffer
	{
		SMART_PTR(CBuffer)
		NO_COPY(CBuffer)

	private:
		AutoReleasePtr<ID3D11Buffer> m_buffer;
		UINT m_bufferSize;

	public:
		CBuffer(Graphics& graphics, UINT sizeInBytes);

		bool WriteBuffer(Graphics& graphics, void* dataptr);
		inline ID3D11Buffer* getBuffer() { return m_buffer; }
		inline UINT getBufferSize() { return m_bufferSize; }
	};
}