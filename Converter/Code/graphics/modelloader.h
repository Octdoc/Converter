#pragma once

#include "shaderbase.h"
#include "math/geometry.h"
#include "math/boundingvolume.h"
#include <fstream>

struct aiScene;

namespace gfx
{
	struct OMDHeader
	{
		char fileFormat;	//'t' for text, 'b' for binary
		char extension[3];	//"OMD"
		UINT vertexLayout;
		UINT shaderType;
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
		std::wstring m_folder;
		std::wstring m_filename;

		UINT m_vertexSizeInBytes;
		UINT m_vertexLayout;
		UINT m_shaderType;
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

	private:
		void Create(Vertex_PTMB vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT shaderType);
		void LoadAssimp(LPCWSTR filename);
		void StoreData(const aiScene *scene);
		void StoreMaterials(const aiScene *scene);
		void StoreVertices(const aiScene *scene);

#pragma region Load OMD

		void LoadOMD(LPCWSTR filename);

		void LoadOMDBinary(LPCWSTR filename);
		void ReadHeaderBinary(std::ifstream& infile, OMDHeader& header);
		void ReadVerticesBinary(std::ifstream& infile, OMDHeader& header);
		void ReadIndicesBinary(std::ifstream& infile, OMDHeader& header);
		void ReadGroupsBinary(std::ifstream& infile, OMDHeader& header);
		void ReadMaterialsBinary(std::ifstream& infile, OMDHeader& header);
		void ReadHitboxBinary(std::ifstream& infile, OMDHeader& header);
		void ReadBonesBinary(std::ifstream& infile, OMDHeader& header);
		void ReadAnimationsBinary(std::ifstream& infile, OMDHeader& header);

		void LoadOMDText(LPCWSTR filename);
		void ReadHeaderText(std::wifstream& infile, OMDHeader& header);
		void ReadVerticesText(std::wifstream& infile, OMDHeader& header);
		void ReadIndicesText(std::wifstream& infile, OMDHeader& header);
		void ReadGroupsText(std::wifstream& infile, OMDHeader& header);
		void ReadMaterialsText(std::wifstream& infile, OMDHeader& header);
		void ReadHitboxText(std::wifstream& infile, OMDHeader& header);
		void ReadBonesText(std::wifstream& infile, OMDHeader& header);
		void ReadAnimationsText(std::wifstream& infile, OMDHeader& header);

#pragma endregion

#pragma region Export OMD

		void ExportOMDBinary(UINT shaderType, LPCWSTR filename);
		void WriteHeaderBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteVerticesBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteIndicesBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteGroupsBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteMaterialsBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteHitboxBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteBonesBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteAnimationsBinary(std::ofstream& outfile, OMDHeader& header);

		void ExportOMDText(UINT shaderType, LPCWSTR filename);
		void WriteHeaderText(std::wofstream& outfile, OMDHeader& header);
		void WriteVerticesText(std::wofstream& outfile, OMDHeader& header);
		void WriteIndicesText(std::wofstream& outfile, OMDHeader& header);
		void WriteGroupsText(std::wofstream& outfile, OMDHeader& header);
		void WriteMaterialsText(std::wofstream& outfile, OMDHeader& header);
		void WriteHitboxText(std::wofstream& outfile, OMDHeader& header);
		void WriteBonesText(std::wofstream& outfile, OMDHeader& header);
		void WriteAnimationsText(std::wofstream& outfile, OMDHeader& header);

#pragma endregion

	public:
		ModelLoader();
		ModelLoader(LPCWSTR filename);

		void Clear();
		void ExportOMD(UINT shaderType, LPCWSTR filename, bool binary = true);

		void LoadModel(LPCWSTR filename);
		void CreateCube(mth::float3 position, mth::float3 size, UINT shaderType);
		void CreateFullScreenQuad();
		void CreateScreenQuad(mth::float2 pos, mth::float2 size);
		void CreateQuad(mth::float2 pos, mth::float2 size, UINT shaderType);
		void CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT shaderType);

		void FlipInsideOut();
		void Transform(mth::float4x4 transform);

		inline std::wstring& getFolderName() { return m_folder; }
		inline std::wstring& getFilename() { return m_filename; }
		inline VertexElement* getVertices() { return m_vertices.data(); }
		inline UINT getVertexCount() { return (UINT)(m_vertices.size() / (m_vertexSizeInBytes / sizeof(float))); }
		inline UINT* getIndices() { return m_indices.data(); }
		inline UINT getIndexCount() { return (UINT)m_indices.size(); }
		inline UINT getVertexLayout() { return m_vertexLayout; }
		inline UINT getShaderType() { return m_shaderType; }
		inline UINT getVertexSizeInBytes() { return m_vertexSizeInBytes; }
		inline UINT getVertexSizeInFloats() { return m_vertexSizeInBytes / sizeof(float); }
		inline VertexGroup& getVertexGroup(UINT index) { return m_groups[index]; }
		inline UINT getVertexGroupCount() { return (UINT)m_groups.size(); }
		inline UINT getMaterialCount() { return (UINT)m_textureNames.size(); }
		inline std::wstring& getTexture(UINT index) { return m_textureNames[index]; }
		inline std::wstring& getNormalmap(UINT index) { return m_normalmapNames[index]; }
	};
}