#pragma once

#include "graphics.h"
#include "math/linalg.h"

namespace gfx
{
	AutoReleasePtr<ID3DBlob> LoadShaderCode(LPCWSTR filename);
	AutoReleasePtr<ID3DBlob> CompileShader(std::string& shaderCode, const char* entry, const char* target, const wchar_t* errorOutputFile = nullptr);

	struct VertexGroup
	{
		UINT startIndex;
		UINT indexCount;
		int materialIndex;
	};

	namespace ModelType
	{
		enum Part :UINT {
			POSITION = 1 << 0,
			TEXCOORD = 1 << 1,
			NORMAL = 1 << 2,
			TANGENT_BINORMAL = 1 << 3,
			BONE = 1 << 4,
			TEXTURE = 1 << 5,
			NORMALMAP = 1 << 6
		};

		const UINT P = POSITION;
		const UINT PT = POSITION | TEXCOORD | TEXTURE;
		const UINT PN = POSITION | NORMAL;
		const UINT PTN = POSITION | TEXCOORD | NORMAL | TEXTURE;
		const UINT PM = POSITION | TEXCOORD | NORMAL | TANGENT_BINORMAL | NORMALMAP;
		const UINT PTM = POSITION | TEXCOORD | NORMAL | TANGENT_BINORMAL | TEXTURE | NORMALMAP;
		const UINT PB = POSITION | BONE;
		const UINT PTB = POSITION | TEXCOORD | TEXTURE | BONE;
		const UINT PNB = POSITION | NORMAL | BONE;
		const UINT PTNB = POSITION | TEXCOORD | NORMAL | TEXTURE | BONE;
		const UINT PMB = POSITION | TEXCOORD | NORMAL | TANGENT_BINORMAL | NORMALMAP | BONE;
		const UINT PTMB = POSITION | TEXCOORD | NORMAL | TANGENT_BINORMAL | TEXTURE | NORMALMAP | BONE;
		const UINT AllPart = POSITION | TEXCOORD | NORMAL | TANGENT_BINORMAL | TEXTURE | NORMALMAP | BONE;

		inline bool HasPositions(UINT modelType) { return (bool)(modelType & Part::POSITION); }
		inline bool HasTexcoords(UINT modelType) { return (bool)(modelType & Part::TEXCOORD); }
		inline bool HasNormals(UINT modelType) { return (bool)(modelType & Part::NORMAL); }
		inline bool HasTangentsBinormals(UINT modelType) { return (bool)(modelType & Part::TANGENT_BINORMAL); }
		inline bool HasBones(UINT modelType) { return (bool)(modelType & Part::BONE); }
		inline bool HasTexture(UINT modelType) { return (bool)(modelType & Part::TEXTURE); }
		inline bool HasNormalmap(UINT modelType) { return (bool)(modelType & Part::NORMALMAP); }
		UINT VertexSizeInBytes(UINT modelType);
		UINT VertexSizeInVertexElements(UINT modelType);
		UINT PositionOffset(UINT modelType);
		UINT TexCoordOffset(UINT modelType);
		UINT NormalOffset(UINT modelType);
		UINT TangentOffset(UINT modelType);
		UINT BinormalOffset(UINT modelType);
		UINT BoneWeightsOffset(UINT modelType);
		UINT BoneIndexOffset(UINT modelType);
		UINT RemoveUnnecessary(UINT modelType);
		UINT ToModelType(bool position, bool texcoord, bool normal, bool tangent_binormal, bool bone, bool texture, bool normalmap);
		inline UINT VertexLayout(UINT modelType) { return modelType & (Part::POSITION | Part::TEXCOORD | Part::NORMAL | Part::TANGENT_BINORMAL | Part::BONE); }
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