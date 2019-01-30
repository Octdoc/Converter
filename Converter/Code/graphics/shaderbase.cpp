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

#pragma region VertexLayout

	VertexLayout::VertexLayout(Value value) :m_value(value) {}
	bool VertexLayout::operator==(Value value)
	{
		return m_value == value;
	}
	bool VertexLayout::operator!=(Value value)
	{
		return m_value != value;
	}
	VertexLayout::operator UINT()
	{
		return (UINT)m_value;
	}
	bool VertexLayout::HasPositions(UINT vertexLayout)
	{
		return (bool)(vertexLayout & Value::POSITION);
	}
	bool VertexLayout::HasTexcoords(UINT vertexLayout)
	{
		return (bool)(vertexLayout & Value::TEXCOORD);
	}
	bool VertexLayout::HasNormals(UINT vertexLayout)
	{
		return (bool)(vertexLayout & Value::NORMAL);
	}
	bool VertexLayout::HasTangentsBinormals(UINT vertexLayout)
	{
		return (bool)(vertexLayout & Value::TANGENT_BINORMAL);
	}
	bool VertexLayout::HasBones(UINT vertexLayout)
	{
		return (bool)(vertexLayout & Value::BONE);
	}
	UINT VertexLayout::VertexSizeInBytes(UINT vertexLayout)
	{
		return VertexSizeInVertexElements(vertexLayout) * sizeof(VertexElement);
	}
	UINT VertexLayout::VertexSizeInVertexElements(UINT vertexLayout)
	{
		UINT size = 0;
		if (HasPositions(vertexLayout))
			size += 3;
		if (HasTexcoords(vertexLayout))
			size += 2;
		if (HasNormals(vertexLayout))
			size += 3;
		if (HasTangentsBinormals(vertexLayout))
			size += 6;
		if (HasBones(vertexLayout))
			size += 8;
		return size;
	}
	UINT VertexLayout::PositionOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		return offset;
	}
	UINT VertexLayout::TexCoordOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		if (HasPositions(vertexLayout))
			offset += 3;
		return offset;
	}
	UINT VertexLayout::NormalOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		if (HasPositions(vertexLayout))
			offset += 3;
		if (HasTexcoords(vertexLayout))
			offset += 2;
		return offset;
	}
	UINT VertexLayout::TangentOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		if (HasPositions(vertexLayout))
			offset += 3;
		if (HasTexcoords(vertexLayout))
			offset += 2;
		if (HasNormals(vertexLayout))
			offset += 3;
		return offset;
	}
	UINT VertexLayout::BinormalOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		if (HasPositions(vertexLayout))
			offset += 3;
		if (HasTexcoords(vertexLayout))
			offset += 2;
		if (HasNormals(vertexLayout))
			offset += 3;
		if (HasTangentsBinormals(vertexLayout))
			offset += 3;
		return offset;
	}
	UINT VertexLayout::BoneWeightsOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		if (HasPositions(vertexLayout))
			offset += 3;
		if (HasTexcoords(vertexLayout))
			offset += 2;
		if (HasNormals(vertexLayout))
			offset += 3;
		if (HasTangentsBinormals(vertexLayout))
			offset += 6;
		return offset;
	}
	UINT VertexLayout::BoneIndexOffset(UINT vertexLayout)
	{
		UINT offset = 0;
		if (HasPositions(vertexLayout))
			offset += 3;
		if (HasTexcoords(vertexLayout))
			offset += 2;
		if (HasNormals(vertexLayout))
			offset += 3;
		if (HasTangentsBinormals(vertexLayout))
			offset += 6;
		if (HasBones(vertexLayout))
			offset += 4;
		return offset;
	}

#pragma endregion

#pragma region MaterialType

	ShaderType::ShaderType() :m_value(Value::ERROR_TYPE) {}
	ShaderType::ShaderType(Value value) : m_value(value) {}
	ShaderType ShaderType::operator=(Value value)
	{
		m_value = value;
		return value;
	}
	bool ShaderType::operator==(Value value)
	{
		return m_value == value;
	}
	bool ShaderType::operator!=(Value value)
	{
		return m_value != value;
	}
	ShaderType::operator UINT()
	{
		return (UINT)m_value;
	}
	bool ShaderType::HasPositions(UINT shaderType)
	{
		return shaderType != Value::ERROR_TYPE;
	}
	bool ShaderType::HasTexture(UINT shaderType)
	{
		return
			shaderType == Value::PT ||
			shaderType == Value::PTN ||
			shaderType == Value::PTM ||
			shaderType == Value::PTB ||
			shaderType == Value::PTNB ||
			shaderType == Value::PTMB;
	}
	bool ShaderType::HasNormals(UINT shaderType)
	{
		return
			shaderType == Value::PN ||
			shaderType == Value::PTN ||
			shaderType == Value::PM ||
			shaderType == Value::PTM ||
			shaderType == Value::PNB ||
			shaderType == Value::PTNB ||
			shaderType == Value::PMB ||
			shaderType == Value::PTMB;
	}
	bool ShaderType::HasNormalmap(UINT shaderType)
	{
		return
			shaderType == Value::PM ||
			shaderType == Value::PTM ||
			shaderType == Value::PMB ||
			shaderType == Value::PTMB;
	}
	bool ShaderType::HasBones(UINT shaderType)
	{
		return
			shaderType == Value::PB ||
			shaderType == Value::PTB ||
			shaderType == Value::PNB ||
			shaderType == Value::PTNB ||
			shaderType == Value::PMB ||
			shaderType == Value::PTMB;
	}
	UINT ShaderType::ToVertexLayout(UINT shaderType)
	{
		UINT vertexLayout[] = {
			0,
			VertexLayout::POSITION,
			VertexLayout::POSITION | VertexLayout::TEXCOORD,
			VertexLayout::POSITION | VertexLayout::NORMAL,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::NORMAL,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::NORMAL | VertexLayout::TANGENT_BINORMAL,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::NORMAL | VertexLayout::TANGENT_BINORMAL,
			VertexLayout::POSITION | VertexLayout::BONE,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::BONE,
			VertexLayout::POSITION | VertexLayout::NORMAL | VertexLayout::BONE,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::NORMAL | VertexLayout::BONE,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::NORMAL | VertexLayout::TANGENT_BINORMAL | VertexLayout::BONE,
			VertexLayout::POSITION | VertexLayout::TEXCOORD | VertexLayout::NORMAL | VertexLayout::TANGENT_BINORMAL | VertexLayout::BONE
		};
		return vertexLayout[shaderType];
	}
	UINT ShaderType::ToShaderType(bool p, bool t, bool n, bool m, bool b)
	{
		if (p && !t && !n && !m)
			return b ? PB : P;
		if (p && t && !n && !m)
			return b ? PTB : PT;
		if (p && !t && n && !m)
			return b ? PNB : PN;
		if (p && t && n && !m)
			return b ? PTNB : PTN;
		if (p && !t && n && m)
			return b ? PMB : PM;
		if (p && t && n && m)
			return b ? PTMB : PTM;
		return ERROR_TYPE;
	}

#pragma endregion
}