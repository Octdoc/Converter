#include "assimploader.h"
#include "pmxloader.h"
#include "omdloader.h"
#include "omdexporter.h"

namespace gfx
{
	void ModelLoader::OrganizeMaterials()
	{
		std::vector<std::wstring> textureNames, normalmapNames;
		for (UINT i = 0; i < (UINT)m_groups.size(); i++)
		{
			textureNames.push_back(m_textureNames[m_groups[i].materialIndex]);
			normalmapNames.push_back(m_normalmapNames[m_groups[i].materialIndex]);
			m_groups[i].materialIndex = i;
		}
		m_textureNames.swap(textureNames);
		m_normalmapNames.swap(normalmapNames);
	}

	void ModelLoader::Create(Vertex_PTMB vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT modelType)
	{
		int counter = 0;
		m_modelType = modelType;
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize(vertexCount * getVertexSizeInFloats());
		m_indices.resize(indexCount);
		for (UINT v = 0; v < vertexCount; v++)
		{
			if (ModelType::HasPositions(modelType))
			{
				m_vertices[counter++] = vertices[v].position.x;
				m_vertices[counter++] = vertices[v].position.y;
				m_vertices[counter++] = vertices[v].position.z;
			}
			if (ModelType::HasTexcoords(modelType))
			{
				m_vertices[counter++] = vertices[v].texcoord.x;
				m_vertices[counter++] = vertices[v].texcoord.y;
			}
			if (ModelType::HasNormals(modelType))
			{
				m_vertices[counter++] = vertices[v].normal.x;
				m_vertices[counter++] = vertices[v].normal.y;
				m_vertices[counter++] = vertices[v].normal.z;
			}
			if (ModelType::HasTangentsBinormals(modelType))
			{
				m_vertices[counter++] = vertices[v].tangent.x;
				m_vertices[counter++] = vertices[v].tangent.y;
				m_vertices[counter++] = vertices[v].tangent.z;
				m_vertices[counter++] = vertices[v].binormal.x;
				m_vertices[counter++] = vertices[v].binormal.y;
				m_vertices[counter++] = vertices[v].binormal.z;
			}
			if (ModelType::HasBones(modelType))
			{
				m_vertices[counter++] = vertices[v].boneWeights[0];
				m_vertices[counter++] = vertices[v].boneWeights[1];
				m_vertices[counter++] = vertices[v].boneWeights[2];
				m_vertices[counter++] = vertices[v].boneWeights[3];
				m_vertices[counter++] = vertices[v].boneIndex[0];
				m_vertices[counter++] = vertices[v].boneIndex[1];
				m_vertices[counter++] = vertices[v].boneIndex[2];
				m_vertices[counter++] = vertices[v].boneIndex[3];
			}
		}
		for (UINT i = 0; i < indexCount; i++)
			m_indices[i] = indices[i];
		m_groups.push_back({ 0, indexCount, 0 });
		m_textureNames.push_back(L"");
		m_normalmapNames.push_back(L"");
	}

	ModelLoader::ModelLoader() :
		m_vertexSizeInBytes(0),
		m_modelType(0),
		m_boundingVolumeType(0),
		m_bvSphereRadius(0.0f) {}
	ModelLoader::ModelLoader(LPCWSTR filename, UINT modelType) :
		m_vertexSizeInBytes(0),
		m_modelType(0),
		m_boundingVolumeType(0)
	{
		LoadModel(filename, modelType);
	}
	void ModelLoader::Clear()
	{
		m_folder.clear();
		m_filename.clear();
		m_vertexSizeInBytes = 0;
		m_modelType = 0;
		m_boundingVolumeType = 0;
		m_vertices.clear();
		m_indices.clear();
		m_textureNames.clear();
		m_normalmapNames.clear();
		m_groups.clear();
		m_bvPosition = mth::float3();
		m_bvCuboidSize = mth::float3();
		m_bvSphereRadius = 0.0f;
		m_hitbox.clear();
	}
	void ModelLoader::ExportOMD(LPCWSTR filename, UINT modelType, bool binary)
	{
		if (binary)
			((OMDExporter*)this)->ExportOMDBinary(filename, modelType);
		else
			((OMDExporter*)this)->ExportOMDText(filename, modelType);
	}
	void ModelLoader::LoadModel(LPCWSTR filename, UINT modelType)
	{
		std::wstring path;
		for (int i = 0; filename[i]; i++)
		{
			if (filename[i] != '\"')
				path += filename[i];
		}
		UINT lastSlashIndex = 0;
		UINT lastDotIndex = 0;
		UINT i;
		for (i = 0; path[i]; i++)
		{
			if (path[i] == '/' || path[i] == '\\')
				lastSlashIndex = i;
			if (path[i] == '.')
				lastDotIndex = i;
		}
		for (i = 0; i <= lastSlashIndex; i++)
			m_folder += path[i];
		while (i < lastDotIndex)
			m_filename += path[i++];

		if (path[i + 0] == '.' &&
			path[i + 1] == 'o' &&
			path[i + 2] == 'm' &&
			path[i + 3] == 'd' &&
			path[i + 4] == '\0')
			((OMDLoader*)this)->LoadOMD(path.c_str(), modelType);
		else if (path[i + 0] == '.' &&
			path[i + 1] == 'p' &&
			path[i + 2] == 'm' &&
			path[i + 3] == 'x' &&
			path[i + 4] == '\0')
			((PMXLoader*)this)->LoadPMX(path.c_str(), modelType);
		else
			((AssimpLoader*)this)->LoadAssimp(path.c_str(), modelType);
		OrganizeMaterials();
	}

#pragma region Create primitives

	void ModelLoader::CreateCube(mth::float3 p, mth::float3 s, UINT modelType)
	{
		Vertex_PTMB vertices[] = {
			{{p.x + s.x, p.y, p.z + s.z},			{1.0f, 1.0f},	{0.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y, p.z},					{1.0f, 0.0f},	{0.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z},						{0.0f, 0.0f},	{0.0f, -1.0f,0.0f},		{0.0f, 0.0f, -1.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z + s.z},					{0.0f, 1.0f},	{0.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z},		{1.0f, 0.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f},	{0.0f, 0.0f, -1.0f}},
			{{p.x, p.y + s.y, p.z + s.z},			{0.0f, 0.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f},	{0.0f, 0.0f, -1.0f}},
			{{p.x, p.y + s.y, p.z},					{0.0f, 1.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f},	{0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z},			{1.0f, 1.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f},	{0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z + s.z},			{1.0f, 1.0f},	{1.0f, 0.0f, 0.0f},		{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z},		{1.0f, 0.0f},	{1.0f, 0.0f, 0.0f},		{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z},			{0.0f, 0.0f},	{1.0f, 0.0f, 0.0f},		{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z},					{0.0f, 1.0f},	{1.0f, 0.0f, 0.0f},		{0.0f, 1.0f, 0.0f},		{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z},					{1.0f, 1.0f},	{0.0f, 0.0f, -1.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y + s.y, p.z},			{1.0f, 0.0f},	{0.0f, 0.0f, -1.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y + s.y, p.z},					{0.0f, 0.0f},	{0.0f, 0.0f, -1.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z},						{0.0f, 1.0f},	{0.0f, 0.0f, -1.0f},	{0.0f, 1.0f, 0.0f},		{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z},						{1.0f, 1.0f},	{-1.0f, 0.0f, 0.0f},	{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},
			{{p.x, p.y + s.y, p.z},					{1.0f, 0.0f},	{-1.0f, 0.0f, 0.0f},	{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},
			{{p.x, p.y + s.y, p.z + s.z},			{0.0f, 0.0f},	{-1.0f, 0.0f, 0.0f},	{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},
			{{p.x, p.y, p.z + s.z},					{0.0f, 1.0f},	{-1.0f, 0.0f, 0.0f},	{0.0f, 1.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z},		{0.0f, 0.0f},	{0.0f, 0.0f, 1.0f},		{0.0f, -1.0f, 0.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y, p.z + s.z},			{0.0f, 1.0f},	{0.0f, 0.0f, 1.0f},		{0.0f, -1.0f, 0.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z + s.z},					{1.0f, 1.0f},	{0.0f, 0.0f, 1.0f},		{0.0f, -1.0f, 0.0f},	{-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y + s.y, p.z + s.z},			{1.0f, 0.0f},	{0.0f, 0.0f, 1.0f},		{0.0f, -1.0f, 0.0f},	{-1.0f, 0.0f, 0.0f}}
		};
		UINT indices[] = {
			0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6, 8, 10, 9, 8, 11, 10, 12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22
		};
		Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), modelType);
	}

	void ModelLoader::CreateFullScreenQuad()
	{
		CreateQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ModelType::PT);
		Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f) * mth::float4x4::RotationX(-mth::pi * 0.5f));
	}

	void ModelLoader::CreateScreenQuad(mth::float2 pos, mth::float2 size)
	{
		CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ModelType::PT);
		Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f) * mth::float4x4::RotationX(-mth::pi * 0.5f));
	}

	void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, UINT modelType)
	{
		CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, modelType);
	}

	void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT modelType)
	{
		Vertex_PTMB vertices[] = {
		{ {pos.x + size.x, 0.0f, pos.y}, { tpos.x + tsize.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {pos.x + size.x, 0.0f, pos.y + size.y}, { tpos.x + tsize.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {pos.x, 0.0f, pos.y + size.y}, { tpos.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {pos.x, 0.0f, pos.y}, { tpos.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
		};
		UINT indices[] = { 0, 2, 1, 0, 3, 2 };
		Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), modelType);
	}

#pragma endregion

	void ModelLoader::MakeHitboxFromVertices()
	{
		mth::float3 maxpos, minpos;
		m_hitbox.clear();
		m_boundingVolumeType = mth::BoundingVolume::CUBOID;
		m_bvSphereRadius = 0.0f;
		minpos.x = maxpos.x = m_vertices[0].f;
		minpos.y = maxpos.y = m_vertices[1].f;
		minpos.z = maxpos.z = m_vertices[2].f;
		UINT vertexSize = getVertexSizeInFloats();
		m_hitbox.resize(m_indices.size() / 3);
		mth::float3 tri[3];
		for (UINT i = 0; i < (UINT)m_hitbox.size(); i++)
		{
			tri[0] = mth::float3(
				m_vertices[vertexSize * m_indices[i * 3 + 0] + 0].f,
				m_vertices[vertexSize * m_indices[i * 3 + 0] + 1].f,
				m_vertices[vertexSize * m_indices[i * 3 + 0] + 2].f);
			tri[1] = mth::float3(
				m_vertices[vertexSize * m_indices[i * 3 + 1] + 0].f,
				m_vertices[vertexSize * m_indices[i * 3 + 1] + 1].f,
				m_vertices[vertexSize * m_indices[i * 3 + 1] + 2].f);
			tri[2] = mth::float3(
				m_vertices[vertexSize * m_indices[i * 3 + 2] + 0].f,
				m_vertices[vertexSize * m_indices[i * 3 + 2] + 1].f,
				m_vertices[vertexSize * m_indices[i * 3 + 2] + 2].f);
			for (UINT v = 0; v < 3; v++)
			{
				if (minpos.x > tri[v].x) minpos.x = tri[v].x;
				if (maxpos.x < tri[v].x) maxpos.x = tri[v].x;
				if (minpos.y > tri[v].x) minpos.y = tri[v].x;
				if (maxpos.y < tri[v].x) maxpos.y = tri[v].x;
				if (minpos.z > tri[v].x) minpos.z = tri[v].x;
				if (maxpos.z < tri[v].x) maxpos.z = tri[v].x;
			}
			m_hitbox[i] = mth::Triangle(tri);
		}
		m_bvPosition = minpos;
		m_bvCuboidSize = maxpos - minpos;
	}

	void ModelLoader::MakeVerticesFromHitbox()
	{
		m_modelType = ModelType::P;
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize(m_hitbox.size() * 3 * 3);
		for (size_t i = 0; i < m_hitbox.size(); i++)
		{
			m_vertices[9 * i + 0] = m_hitbox[i].getVertex(0).x;
			m_vertices[9 * i + 1] = m_hitbox[i].getVertex(0).y;
			m_vertices[9 * i + 2] = m_hitbox[i].getVertex(0).z;
			m_vertices[9 * i + 3] = m_hitbox[i].getVertex(1).x;
			m_vertices[9 * i + 4] = m_hitbox[i].getVertex(1).y;
			m_vertices[9 * i + 5] = m_hitbox[i].getVertex(1).z;
			m_vertices[9 * i + 6] = m_hitbox[i].getVertex(2).x;
			m_vertices[9 * i + 7] = m_hitbox[i].getVertex(2).y;
			m_vertices[9 * i + 8] = m_hitbox[i].getVertex(2).z;
		}
		m_indices.resize(m_hitbox.size() * 3);
		for (int i = 0; i < (int)m_indices.size(); i++)
			m_indices[i] = i;
		m_groups.clear();
		m_groups.push_back({ 0, (UINT)m_indices.size() , 0 });
		m_textureNames.clear();
		m_textureNames.push_back(L"");
		m_normalmapNames.clear();
		m_normalmapNames.push_back(L"");
	}

	bool ModelLoader::HasHitbox()
	{
		return m_hitbox.size() > 0;
	}

	void ModelLoader::SwapHitboxes(ModelLoader& other)
	{
		std::swap(other.m_boundingVolumeType, m_boundingVolumeType);
		std::swap(other.m_bvPosition, m_bvPosition);
		std::swap(other.m_bvCuboidSize, m_bvCuboidSize);
		std::swap(other.m_bvSphereRadius, m_bvSphereRadius);
		other.m_hitbox.swap(m_hitbox);
	}

	void ModelLoader::FlipInsideOut()
	{
		for (size_t i = 0; i < m_indices.size(); i += 3)
		{
			UINT tmp = m_indices[i + 1];
			m_indices[i + 1] = m_indices[i + 2];
			m_indices[i + 2] = tmp;
		}
		UINT vertexSize = getVertexSizeInFloats();
		if (ModelType::HasNormals(m_modelType))
		{
			UINT offset = ModelType::NormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				m_vertices[i * vertexSize + offset + 0].f *= -1.0f;
				m_vertices[i * vertexSize + offset + 1].f *= -1.0f;
				m_vertices[i * vertexSize + offset + 2].f *= -1.0f;
			}
		}
		if (ModelType::HasTangentsBinormals(m_modelType))
		{
			UINT offset1 = ModelType::TangentOffset(m_modelType);
			UINT offset2 = ModelType::BinormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				m_vertices[i * vertexSize + offset1 + 0].f *= -1.0f;
				m_vertices[i * vertexSize + offset1 + 1].f *= -1.0f;
				m_vertices[i * vertexSize + offset1 + 2].f *= -1.0f;
				m_vertices[i * vertexSize + offset2 + 0].f *= -1.0f;
				m_vertices[i * vertexSize + offset2 + 1].f *= -1.0f;
				m_vertices[i * vertexSize + offset2 + 2].f *= -1.0f;
			}
		}
	}

	void ModelLoader::Transform(mth::float4x4 transform)
	{
		UINT vertexSize = getVertexSizeInFloats();
		mth::float4 v;
		if (ModelType::HasPositions(m_modelType))
		{
			UINT offset = ModelType::PositionOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i * vertexSize + offset + 0].f;
				v.y = m_vertices[i * vertexSize + offset + 1].f;
				v.z = m_vertices[i * vertexSize + offset + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset + 0] = v.x;
				m_vertices[i * vertexSize + offset + 1] = v.y;
				m_vertices[i * vertexSize + offset + 2] = v.z;
			}
		}
		if (ModelType::HasNormals(m_modelType))
		{
			UINT offset = ModelType::NormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i * vertexSize + offset + 0].f;
				v.y = m_vertices[i * vertexSize + offset + 1].f;
				v.z = m_vertices[i * vertexSize + offset + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset + 0] = v.x;
				m_vertices[i * vertexSize + offset + 1] = v.y;
				m_vertices[i * vertexSize + offset + 2] = v.z;
			}
		}
		if (ModelType::HasTangentsBinormals(m_modelType))
		{
			UINT offset1 = ModelType::TangentOffset(m_modelType);
			UINT offset2 = ModelType::BinormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i * vertexSize + offset1 + 0].f;
				v.y = m_vertices[i * vertexSize + offset1 + 1].f;
				v.z = m_vertices[i * vertexSize + offset1 + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset1 + 0] = v.x;
				m_vertices[i * vertexSize + offset1 + 1] = v.y;
				m_vertices[i * vertexSize + offset1 + 2] = v.z;

				v.x = m_vertices[i * vertexSize + offset2 + 0].f;
				v.y = m_vertices[i * vertexSize + offset2 + 1].f;
				v.z = m_vertices[i * vertexSize + offset2 + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset2 + 0] = v.x;
				m_vertices[i * vertexSize + offset2 + 1] = v.y;
				m_vertices[i * vertexSize + offset2 + 2] = v.z;
			}
		}
	}
}