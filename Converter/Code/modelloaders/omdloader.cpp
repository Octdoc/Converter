#include "omdloader.h"

namespace gfx
{
	void OMDLoader::LoadOMD(LPCWSTR filename, UINT modelType)
	{
		std::ifstream infile(filename, std::ios::in | std::ios::binary);
		if (!infile.good())
			throw std::exception(std::string("Failed to load file: " + ToStr(filename)).c_str());
		char filedata[4];
		infile.read(filedata, sizeof(filedata));
		infile.close();
		if ((filedata[1] != 'o' && filedata[1] != 'O') ||
			(filedata[2] != 'm' && filedata[2] != 'M') ||
			(filedata[3] != 'd' && filedata[3] != 'D'))
			throw std::exception(std::string("Corrupted file: " + ToStr(filename)).c_str());
		if (filedata[0] == 't' || filedata[0] == 'T')
			LoadOMDText(filename, modelType);
		else if (filedata[0] == 'b' || filedata[0] == 'B')
			LoadOMDBinary(filename, modelType);
		else
			throw std::exception(std::string("Corrupted file: " + ToStr(filename)).c_str());
	}

#pragma region Load binary

	void OMDLoader::LoadOMDBinary(LPCWSTR filename, UINT modelType)
	{
		std::ifstream infile(filename, std::ios::in | std::ios::binary);
		OMDHeader header;
		ReadHeaderBinary(infile, header, modelType);
		ReadVerticesBinary(infile, header);
		ReadIndicesBinary(infile, header);
		ReadGroupsBinary(infile, header);
		ReadMaterialsBinary(infile, header);
		ReadHitboxBinary(infile, header);
		ReadBonesBinary(infile, header);
		ReadAnimationsBinary(infile, header);
	}
	void OMDLoader::ReadHeaderBinary(std::ifstream& infile, OMDHeader& header, UINT modelType)
	{
		infile.read((char*)& header, sizeof(header));
		m_modelType = ModelType::RemoveUnnecessary(header.modelType & modelType);
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_boundingVolumeType = header.boundingVolumePrimitive;
	}
	void OMDLoader::ReadVerticesBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_vertices.resize(header.vertexCount * m_vertexSizeInBytes / sizeof(VertexElement));
		if (ModelType::VertexLayout(m_modelType) == ModelType::VertexLayout(header.modelType))
			infile.read((char*)m_vertices.data(), header.vertexCount * m_vertexSizeInBytes);
		else
		{
			VertexElement tmp[8];
			UINT vertexCounter = 0;
			for (UINT i = 0; i < header.vertexCount; i++)
			{
				if (ModelType::HasPositions(header.modelType))
				{
					infile.read((char*)tmp, 3 * sizeof(VertexElement));
					if (ModelType::HasPositions(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
					}
				}
				if (ModelType::HasTexcoords(header.modelType))
				{
					infile.read((char*)tmp, 2 * sizeof(VertexElement));
					if (ModelType::HasTexcoords(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
					}
				}
				if (ModelType::HasNormals(header.modelType))
				{
					infile.read((char*)tmp, 3 * sizeof(VertexElement));
					if (ModelType::HasNormals(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
					}
				}
				if (ModelType::HasTangentsBinormals(header.modelType))
				{
					infile.read((char*)tmp, 6 * sizeof(VertexElement));
					if (ModelType::HasTangentsBinormals(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
						m_vertices[vertexCounter++] = tmp[3];
						m_vertices[vertexCounter++] = tmp[4];
						m_vertices[vertexCounter++] = tmp[5];
					}
				}
				if (ModelType::HasBones(header.modelType))
				{
					infile.read((char*)tmp, 8 * sizeof(VertexElement));
					if (ModelType::HasBones(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
						m_vertices[vertexCounter++] = tmp[3];
						m_vertices[vertexCounter++] = tmp[4];
						m_vertices[vertexCounter++] = tmp[5];
						m_vertices[vertexCounter++] = tmp[6];
						m_vertices[vertexCounter++] = tmp[7];
					}
				}
			}
		}
	}
	void OMDLoader::ReadIndicesBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_indices.resize(header.indexCount);
		infile.read((char*)m_indices.data(), header.indexCount * sizeof(UINT));
	}
	void OMDLoader::ReadGroupsBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_groups.resize(header.groupCount);
		infile.read((char*)m_groups.data(), header.groupCount * sizeof(VertexGroup));
	}
	void OMDLoader::ReadMaterialsBinary(std::ifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		m_textures.resize(header.materialCount);
		m_normalmaps.resize(header.materialCount);
		for (UINT i = 0; i < header.materialCount; i++)
		{
			infile.read((char*)& ch, sizeof(WCHAR));
			while (ch)
			{
				if (ModelType::HasTexture(m_modelType))
					m_textures[i].filename += ch;
				infile.read((char*)& ch, sizeof(WCHAR));
			}
			infile.read((char*)& ch, sizeof(WCHAR));
			while (ch)
			{
				if (ModelType::HasNormalmap(m_modelType))
					m_normalmaps[i].filename += ch;
				infile.read((char*)& ch, sizeof(WCHAR));
			}
		}
	}
	void OMDLoader::ReadHitboxBinary(std::ifstream& infile, OMDHeader& header)
	{
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			infile.read((char*)& m_bvPosition, sizeof(m_bvPosition));
			infile.read((char*)& m_bvCuboidSize, sizeof(m_bvCuboidSize));
			break;
		case mth::BoundingVolume::SPHERE:
			infile.read((char*)& m_bvPosition, sizeof(m_bvPosition));
			infile.read((char*)& m_bvSphereRadius, sizeof(m_bvSphereRadius));
			break;
		default:
			header.boundingVolumePrimitive = mth::BoundingVolume::NO_TYPE;
			break;
		}
		if (header.hitboxTriangleCount)
		{
			m_hitbox.resize(header.hitboxTriangleCount);
			infile.read((char*)m_hitbox.data(), header.hitboxTriangleCount * sizeof(mth::Triangle));
		}
	}
	void OMDLoader::ReadBonesBinary(std::ifstream& infile, OMDHeader& header)
	{
	}
	void OMDLoader::ReadAnimationsBinary(std::ifstream& infile, OMDHeader& header)
	{
	}

#pragma endregion

#pragma region Load text

	void OMDLoader::LoadOMDText(LPCWSTR filename, UINT modelType)
	{
		std::wifstream infile(filename, std::ios::in);
		OMDHeader header;
		ReadHeaderText(infile, header, modelType);
		ReadVerticesText(infile, header);
		ReadIndicesText(infile, header);
		ReadGroupsText(infile, header);
		ReadMaterialsText(infile, header);
		ReadHitboxText(infile, header);
		ReadBonesText(infile, header);
		ReadAnimationsText(infile, header);
	}
	void OMDLoader::ReadHeaderText(std::wifstream& infile, OMDHeader& header, UINT modelType)
	{
		WCHAR ch;
		infile >> ch;
		header.fileFormat = (char)ch;
		infile >> ch;
		header.extension[0] = (char)ch;
		infile >> ch;
		header.extension[1] = (char)ch;
		infile >> ch;
		header.extension[2] = (char)ch;
		do { infile >> ch; } while (ch != ':');
		infile >> header.modelType;
		do { infile >> ch; } while (ch != ':');
		infile >> header.vertexCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.indexCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.groupCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.materialCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.boundingVolumePrimitive;
		do { infile >> ch; } while (ch != ':');
		infile >> header.hitboxTriangleCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.boneCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.animationCount;
		m_modelType = ModelType::RemoveUnnecessary(header.modelType & modelType);
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_boundingVolumeType = header.boundingVolumePrimitive;
	}
	void OMDLoader::ReadVerticesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_vertices.resize(header.vertexCount * m_vertexSizeInBytes / sizeof(VertexElement));
		VertexElement tmp[8];
		UINT vertexCounter = 0;
		for (UINT i = 0; i < header.vertexCount; i++)
		{
			if (ModelType::HasPositions(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				if (ModelType::HasPositions(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
				}
			}
			if (ModelType::HasTexcoords(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				if (ModelType::HasTexcoords(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
				}
			}
			if (ModelType::HasNormals(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				if (ModelType::HasNormals(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
				}
			}
			if (ModelType::HasTangentsBinormals(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				infile >> tmp[3].f;
				infile >> tmp[4].f;
				infile >> tmp[5].f;
				if (ModelType::HasTangentsBinormals(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
					m_vertices[vertexCounter++] = tmp[3];
					m_vertices[vertexCounter++] = tmp[4];
					m_vertices[vertexCounter++] = tmp[5];
				}
			}
			if (ModelType::HasBones(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				infile >> tmp[3].f;
				infile >> tmp[4].u;
				infile >> tmp[5].u;
				infile >> tmp[6].u;
				infile >> tmp[7].u;
				if (ModelType::HasBones(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
					m_vertices[vertexCounter++] = tmp[3];
					m_vertices[vertexCounter++] = tmp[4];
					m_vertices[vertexCounter++] = tmp[5];
					m_vertices[vertexCounter++] = tmp[6];
					m_vertices[vertexCounter++] = tmp[7];
				}
			}
		}
	}
	void OMDLoader::ReadIndicesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_indices.resize(header.indexCount);
		for (UINT i = 0; i < header.indexCount; i++)
			infile >> m_indices[i];
	}
	void OMDLoader::ReadGroupsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_groups.resize(header.groupCount);
		for (UINT i = 0; i < header.groupCount; i++)
		{
			do { infile >> ch; } while (ch != ':');
			infile >> m_groups[i].startIndex;
			do { infile >> ch; } while (ch != ':');
			infile >> m_groups[i].indexCount;
			do { infile >> ch; } while (ch != ':');
			infile >> m_groups[i].materialIndex;
		}

	}
	void OMDLoader::ReadMaterialsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_textures.resize(header.materialCount);
		m_normalmaps.resize(header.materialCount);
		for (UINT i = 0; i < header.materialCount; i++)
		{
			do { infile >> ch; } while (ch != ':');
			do { infile.read(&ch, 1); } while (ch == ' ');
			for (; ch != '\n'; infile.read(&ch, 1))
				m_textures[i].filename += ch;
			do { infile >> ch; } while (ch != ':');
			do { infile.read(&ch, 1); } while (ch == ' ');
			for (; ch != '\n'; infile.read(&ch, 1))
				m_normalmaps[i].filename += ch;
		}
	}
	void OMDLoader::ReadHitboxText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			infile >> m_bvPosition.x;
			infile >> m_bvPosition.y;
			infile >> m_bvPosition.z;
			infile >> m_bvCuboidSize.x;
			infile >> m_bvCuboidSize.y;
			infile >> m_bvCuboidSize.z;
			break;
		case mth::BoundingVolume::SPHERE:
			infile >> m_bvPosition.x;
			infile >> m_bvPosition.y;
			infile >> m_bvPosition.z;
			infile >> m_bvSphereRadius;
			break;
		default:
			header.boundingVolumePrimitive = mth::BoundingVolume::NO_TYPE;
			break;
		}

		do { infile >> ch; } while (ch != ':');
		if (header.hitboxTriangleCount)
		{
			m_hitbox.resize(header.hitboxTriangleCount);
			mth::float3 tri[3];
			mth::float3 plainNormal;
			float plainDistance;
			for (UINT i = 0; i < header.hitboxTriangleCount; i++)
			{
				infile >> tri[0].x;
				infile >> tri[0].y;
				infile >> tri[0].z;
				infile >> tri[1].x;
				infile >> tri[1].y;
				infile >> tri[1].z;
				infile >> tri[2].x;
				infile >> tri[2].y;
				infile >> tri[2].z;
				infile >> plainNormal.x;
				infile >> plainNormal.y;
				infile >> plainNormal.z;
				infile >> plainDistance;
				m_hitbox[i] = mth::Triangle(tri, plainNormal, plainDistance);
			}
		}
	}
	void OMDLoader::ReadBonesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
	}
	void OMDLoader::ReadAnimationsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
	}

#pragma endregion

}