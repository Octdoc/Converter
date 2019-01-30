#pragma once

#include "graphics.h"
#include "math/linalg.h"

namespace gfx
{
	AutoReleasePtr<ID3DBlob> LoadShaderCode(LPCWSTR filename);

	struct VertexGroup
	{
		UINT startIndex;
		UINT indexCount;
		int materialIndex;
	};

	class VertexLayout
	{
	public:
		enum Value {
			POSITION = 1 << 0,
			TEXCOORD = 1 << 1,
			NORMAL = 1 << 2,
			TANGENT_BINORMAL = 1 << 3,
			BONE = 1 << 4
		};

	private:
		Value m_value;

	public:
		VertexLayout(Value value);
		bool operator==(Value value);
		bool operator!=(Value value);
		operator UINT();

		static bool HasPositions(UINT vertexLayout);
		static bool HasTexcoords(UINT vertexLayout);
		static bool HasNormals(UINT vertexLayout);
		static bool HasTangentsBinormals(UINT vertexLayout);
		static bool HasBones(UINT vertexLayout);
		static UINT VertexSizeInBytes(UINT vertexLayout);
		static UINT VertexSizeInVertexElements(UINT vertexLayout);
		static UINT PositionOffset(UINT vertexLayout);
		static UINT TexCoordOffset(UINT vertexLayout);
		static UINT NormalOffset(UINT vertexLayout);
		static UINT TangentOffset(UINT vertexLayout);
		static UINT BinormalOffset(UINT vertexLayout);
		static UINT BoneWeightsOffset(UINT vertexLayout);
		static UINT BoneIndexOffset(UINT vertexLayout);
	};

	class ShaderType
	{
	public:
		enum Value {
			ERROR_TYPE = 0,
			P = 1,
			PT = 2,
			PN = 3,
			PTN = 4,
			PM = 5,
			PTM = 6,
			PB = 7,
			PTB = 8,
			PNB = 9,
			PTNB = 10,
			PMB = 11,
			PTMB = 12
		};

	private:
		Value m_value;

	public:
		ShaderType();
		ShaderType(Value value);
		ShaderType operator=(Value value);
		bool operator==(Value value);
		bool operator!=(Value value);
		operator UINT();

		static bool HasPositions(UINT shaderType);
		static bool HasTexture(UINT shaderType);
		static bool HasNormals(UINT shaderType);
		static bool HasNormalmap(UINT shaderType);
		static bool HasBones(UINT shaderType);
		static UINT ToVertexLayout(UINT shaderType);
		static UINT ToShaderType(bool p, bool t, bool n, bool m, bool b);
	};

#pragma region Vertex structs

	union VertexElement
	{
		float f;
		UINT u;

		float operator=(float n);
		UINT operator=(UINT n);
	};

	struct Vertex_P
	{
		mth::float3 position;
	};

	struct Vertex_PT
	{
		mth::float3 position;
		mth::float2 texcoord;
	};

	struct Vertex_PN
	{
		mth::float3 position;
		mth::float3 normal;
	};

	struct Vertex_PTN
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
	};

	struct Vertex_PM
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
		mth::float3 tangent;
		mth::float3 binormal;
	};

	struct Vertex_PTM
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
		mth::float3 tangent;
		mth::float3 binormal;
	};

	struct Vertex_PB
	{
		mth::float3 position;
		float boneWeights[4];
		UINT boneIndex[4];
	};

	struct Vertex_PTB
	{
		mth::float3 position;
		mth::float2 textexcoordture;
		float boneWeights[4];
		UINT boneIndex[4];
	};

	struct Vertex_PNB
	{
		mth::float3 position;
		mth::float3 normal;
		float boneWeights[4];
		UINT boneIndex[4];
	};

	struct Vertex_PTNB
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
		float boneWeights[4];
		UINT boneIndex[4];
	};

	struct Vertex_PMB
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
		mth::float3 tangent;
		mth::float3 binormal;
		float boneWeights[4];
		UINT boneIndex[4];
	};

	struct Vertex_PTMB
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
		mth::float3 tangent;
		mth::float3 binormal;
		float boneWeights[4];
		UINT boneIndex[4];
	};

#pragma endregion

}