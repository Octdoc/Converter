#pragma once

#include "graphics/shaderbase.h"
#include "math/geometry.h"
#include "math/boundingvolume.h"
#include <fstream>

namespace gfx
{
	struct MeshGroup
	{
		UINT startIndex;
		UINT indexCount;
		UINT materialIndex;
	};

	struct TextureToLoad
	{
		std::wstring filename;
		int width;
		int height;

		/* pixel (x,y) can be accessed: data[(x+y*width)*4+component]
		where component is: 0 for red, 1 for green, 2 for blue, 3 for alpha */
		std::vector<unsigned char> data;
		bool loaded;	//if true, texture can be created from data, if false, texture can be loaded from <filename> file

		TextureToLoad();
		TextureToLoad(const wchar_t* str);
		void Clear();
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
		std::vector<TextureToLoad> m_textures;
		std::vector<TextureToLoad> m_normalmaps;
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
		inline UINT getMaterialCount() { return (UINT)m_textures.size(); }
		inline TextureToLoad& getTexture(UINT index) { return m_textures[index]; }
		inline TextureToLoad& getNormalmap(UINT index) { return m_normalmaps[index]; }
	};
}