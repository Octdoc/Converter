#pragma once

#include "graphics/shaderbase.h"
#include "math/geometry.h"
#include "math/boundingvolume.h"
#include <fstream>

namespace gfx
{
	struct OMDHeader
	{
		char fileFormat;	//'t' for text, 'b' for binary
		char extension[3];	//"OMD"
		UINT modelType;
		UINT vertexCount;
		UINT indexCount;
		UINT groupCount;
		UINT materialCount;
		UINT boundingVolumePrimitive;
		UINT hitboxTriangleCount;
		UINT boneCount;
		UINT animationCount;
	};

	struct MeshGroup
	{
		UINT startIndex;
		UINT indexCount;
		UINT materialIndex;
	};

	class ModelLoader
	{
	protected:
		std::wstring m_folder;
		std::wstring m_filename;

		UINT m_vertexSizeInBytes;
		UINT m_modelType;
		UINT m_boundingVolumeType;

		std::vector<VertexElement> m_vertices;
		std::vector<UINT> m_indices;
		std::vector<std::wstring> m_textureNames;
		std::vector<std::wstring> m_normalmapNames;
		std::vector<VertexGroup> m_groups;
		mth::float3 m_bvPosition;
		mth::float3 m_bvCuboidSize;
		float m_bvSphereRadius;
		std::vector<mth::Triangle> m_hitbox;

	protected:
		void OrganizeMaterials();
		void Create(Vertex_PTMB vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT modelType);

	public:
		ModelLoader();
		ModelLoader(LPCWSTR filename, UINT modelType = ModelType::AllPart);

		void Clear();
		void ExportOMD(LPCWSTR filename, UINT modelType, bool binary = true);

		void LoadModel(LPCWSTR filename, UINT modelType = ModelType::AllPart);
		void CreateCube(mth::float3 position, mth::float3 size, UINT modelType);
		void CreateFullScreenQuad();
		void CreateScreenQuad(mth::float2 pos, mth::float2 size);
		void CreateQuad(mth::float2 pos, mth::float2 size, UINT modelType);
		void CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT modelType);

		void MakeHitboxFromVertices();
		void MakeVerticesFromHitbox();
		bool HasHitbox();
		void SwapHitboxes(ModelLoader& other);
		void FlipInsideOut();
		void Transform(mth::float4x4 transform);

		inline std::wstring& getFolderName() { return m_folder; }
		inline std::wstring& getFilename() { return m_filename; }
		inline VertexElement* getVertices() { return m_vertices.data(); }
		inline UINT getVertexCount() { return (UINT)(m_vertices.size() / (m_vertexSizeInBytes / sizeof(float))); }
		inline UINT* getIndices() { return m_indices.data(); }
		inline UINT getIndexCount() { return (UINT)m_indices.size(); }
		inline UINT getModelType() { return m_modelType; }
		inline UINT getVertexSizeInBytes() { return m_vertexSizeInBytes; }
		inline UINT getVertexSizeInFloats() { return m_vertexSizeInBytes / sizeof(float); }
		inline VertexGroup& getVertexGroup(UINT index) { return m_groups[index]; }
		inline UINT getVertexGroupCount() { return (UINT)m_groups.size(); }
		inline UINT getMaterialCount() { return (UINT)m_textureNames.size(); }
		inline LPCWSTR getTexture(UINT index) { return (index < (UINT)m_textureNames.size()) ? m_textureNames[index].c_str() : L""; }
		inline LPCWSTR getNormalmap(UINT index) { return (index < (UINT)m_normalmapNames.size()) ? m_normalmapNames[index].c_str() : L""; }
	};
}