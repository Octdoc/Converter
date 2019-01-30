#include "constantbuffer.h"

namespace gfx
{
	CBuffer::CBuffer(Graphics& graphics, UINT sizeInBytes) :
		m_buffer(),
		m_bufferSize(sizeInBytes)
	{
		D3D11_BUFFER_DESC bufferDesc;
		HRESULT hr;
		auto device = graphics.getDevice();

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = m_bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
		if (FAILED(hr))
			throw std::exception("Failed to create constant buffer");
	}
	bool CBuffer::WriteBuffer(Graphics& graphics, void *dataptr)
	{
		auto context = graphics.getContext();
		D3D11_MAPPED_SUBRESOURCE resource;
		if (FAILED(context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
			return false;
		memcpy(resource.pData, dataptr, m_bufferSize);
		context->Unmap(m_buffer, 0);
		return true;
	}
}