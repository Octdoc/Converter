#pragma once

#include "graphics.h"
#include "modelloaders/modelloader.h"

namespace gfx
{
	class Model
	{
		SMART_PTR(Model)

	private:
		AutoReleasePtr<ID3D11Buffer> m_vertexBuffer;
		AutoReleasePtr<ID3D11Buffer> m_indexBuffer;

		UINT m_vertexSizeInBytes;
		UINT m_modelType;
		UINT m_vertexCount;
		UINT m_indexCount;

		struct Group
		{
			UINT startIndex;
			UINT indexCount;
		};
		std::vector<Group> m_groups;

	public:
		Model(Graphics& graphics, ModelLoader& model);
		Model(const Model& other) = default;

		inline UINT getGroupCount() { return (UINT)m_groups.size(); }
		void SetBuffersToRender(Graphics& graphics);
		void DrawGroup(Graphics& graphics, UINT index);
		void RenderAll(Graphics& graphics);
	};
}