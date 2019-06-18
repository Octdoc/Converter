#include "shaderbase.h"
#include <fstream>

namespace gfx
{
	AutoReleasePtr<ID3DBlob> LoadShaderCode(LPCWSTR filename)
	{
		HRESULT result;
		std::fstream f(filename, std::ios::in | std::ios::binary);
		if (!f.good())
			throw std::exception((std::string("Failed to open file: ") + ToStr(filename)).c_str());
		auto begin = f.tellg();
		f.seekg(0, std::ios::end);
		auto end = f.tellg();
		f.seekg(0);
		AutoReleasePtr<ID3DBlob> shaderCode;
		result = D3DCreateBlob((SIZE_T)(end - begin), &shaderCode);
		if (FAILED(result))
			throw std::exception("Failed to create blob");
		f.read((char*)shaderCode->GetBufferPointer(), shaderCode->GetBufferSize());
		f.close();
		return shaderCode;
	}

	AutoReleasePtr<ID3DBlob> CompileShader(std::string& shaderCode, const char* entry, const char* target, const wchar_t* errorOutputFile)
	{
		AutoReleasePtr<ID3DBlob> shaderBuffer;
		AutoReleasePtr<ID3DBlob> errorMessage;
		HRESULT hr = D3DCompile(shaderCode.c_str(), shaderCode.length(), nullptr, nullptr, nullptr, entry, target, D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, errorOutputFile ? &errorMessage : nullptr);
		if (FAILED(hr))
		{
			std::ofstream of(errorOutputFile);
			of.write((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
			throw std::exception("Failed to compile shader.");
		}
		return shaderBuffer;
	}

#pragma region VertexElement

	float VertexElement::operator=(float n)
	{
		f = n;
		return n;
	}
	UINT VertexElement::operator=(UINT n)
	{
		u = n;
		return n;
	}

#pragma endregion

#pragma region ModelType

	namespace ModelType
	{
		UINT VertexSizeInBytes(UINT modelType)
		{
			return VertexSizeInVertexElements(modelType) * sizeof(VertexElement);
		}
		UINT VertexSizeInVertexElements(UINT modelType)
		{
			UINT size = 0;
			if (HasPositions(modelType))
				size += 3;
			if (HasTexcoords(modelType))
				size += 2;
			if (HasNormals(modelType))
				size += 3;
			if (HasTangentsBinormals(modelType))
				size += 6;
			if (HasBones(modelType))
				size += 8;
			return size;
		}
		UINT PositionOffset(UINT modelType)
		{
			UINT offset = 0;
			return offset;
		}
		UINT TexCoordOffset(UINT modelType)
		{
			UINT offset = 0;
			if (HasPositions(modelType))
				offset += 3;
			return offset;
		}
		UINT NormalOffset(UINT modelType)
		{
			UINT offset = 0;
			if (HasPositions(modelType))
				offset += 3;
			if (HasTexcoords(modelType))
				offset += 2;
			return offset;
		}
		UINT TangentOffset(UINT modelType)
		{
			UINT offset = 0;
			if (HasPositions(modelType))
				offset += 3;
			if (HasTexcoords(modelType))
				offset += 2;
			if (HasNormals(modelType))
				offset += 3;
			return offset;
		}
		UINT BinormalOffset(UINT modelType)
		{
			UINT offset = 0;
			if (HasPositions(modelType))
				offset += 3;
			if (HasTexcoords(modelType))
				offset += 2;
			if (HasNormals(modelType))
				offset += 3;
			if (HasTangentsBinormals(modelType))
				offset += 3;
			return offset;
		}
		UINT BoneWeightsOffset(UINT modelType)
		{
			UINT offset = 0;
			if (HasPositions(modelType))
				offset += 3;
			if (HasTexcoords(modelType))
				offset += 2;
			if (HasNormals(modelType))
				offset += 3;
			if (HasTangentsBinormals(modelType))
				offset += 6;
			return offset;
		}
		UINT BoneIndexOffset(UINT modelType)
		{
			UINT offset = 0;
			if (HasPositions(modelType))
				offset += 3;
			if (HasTexcoords(modelType))
				offset += 2;
			if (HasNormals(modelType))
				offset += 3;
			if (HasTangentsBinormals(modelType))
				offset += 6;
			if (HasBones(modelType))
				offset += 4;
			return offset;
		}
		UINT RemoveUnnecessary(UINT modelType)
		{
			if (!HasPositions(modelType))
				return 0;
			if (!HasTexcoords(modelType))
				modelType &= ~(Part::TEXTURE | Part::NORMALMAP | Part::TANGENT_BINORMAL);
			if (!HasNormals(modelType))
				modelType &= ~(Part::TANGENT_BINORMAL | Part::NORMALMAP);
			if (!HasTangentsBinormals(modelType))
				modelType &= ~Part::NORMALMAP;
			if (!HasNormalmap(modelType))
			{
				modelType &= ~Part::TANGENT_BINORMAL;
				if (!HasTexture(modelType))
					modelType &= ~Part::TEXCOORD;
			}
			return modelType;
		}
		UINT ToModelType(bool position, bool texcoord, bool normal, bool tangent_binormal, bool bone, bool texture, bool normalmap)
		{
			UINT modelType = 0;
			if (position)
				modelType |= Part::POSITION;
			if (texcoord)
				modelType |= Part::TEXCOORD;
			if (normal)
				modelType |= Part::NORMAL;
			if (tangent_binormal)
				modelType |= Part::TANGENT_BINORMAL;
			if (bone)
				modelType |= Part::BONE;
			if (texture)
				modelType |= Part::TEXTURE;
			if (normalmap)
				modelType |= Part::NORMALMAP;
			return modelType;
		}
	}

#pragma endregion
}