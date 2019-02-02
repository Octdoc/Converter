#include "model.h"
#include "shaderbase.h"

namespace gfx
{
	Model::Model(Graphics& graphics, ModelLoader& model)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC bufferDesc{};
		D3D11_SUBRESOURCE_DATA subResourceData{};
		auto device = graphics.getDevice();

		m_modelType = model.getModelType();
		m_vertexSizeInBytes = model.getVertexSizeInBytes();
		m_vertexCount = model.getVertexCount();
		m_indexCount = model.getIndexCount();

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = m_vertexCount * m_vertexSizeInBytes;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		subResourceData.pSysMem = model.getVertices();

		hr = device->CreateBuffer(&bufferDesc, &subResourceData, &m_vertexBuffer);
		if (FAILED(hr))
			throw std::exception("Failed to create vertex buffer");

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = m_indexCount * sizeof(UINT);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subResourceData.pSysMem = model.getIndices();

		hr = device->CreateBuffer(&bufferDesc, &subResourceData, &m_indexBuffer);
		if (FAILED(hr))
			throw std::exception("Failed to create index buffer");

		for (UINT i = 0; i < model.getVertexGroupCount(); i++)
		{
			VertexGroup vg = model.getVertexGroup(i);
			m_groups.push_back({ vg.startIndex, vg.indexCount });
		}
	}

	void Model::SetBuffersToRender(Graphics& graphics)
	{
		UINT stride = m_vertexSizeInBytes;
		UINT offset = 0;
		auto context = graphics.getContext();

		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void Model::DrawGroup(Graphics& graphics, UINT index)
	{
		graphics.getContext()->DrawIndexed(m_groups[index].indexCount, m_groups[index].startIndex, 0);
	}

	void Model::RenderAll(Graphics& graphics)
	{
		SetBuffersToRender(graphics);
		graphics.getContext()->DrawIndexed(m_indexCount, 0, 0);
	}
}