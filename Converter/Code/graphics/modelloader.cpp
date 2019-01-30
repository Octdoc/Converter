#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "modelloader.h"

#pragma comment (lib, "Code/assimp/lib/assimp.lib")

namespace gfx
{
	void ModelLoader::Create(Vertex_PTMB vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT shaderType)
	{
		int counter = 0;
		m_shaderType = shaderType;
		m_vertexLayout = ShaderType::ToVertexLayout(shaderType);
		m_vertexSizeInBytes = VertexLayout::VertexSizeInBytes(m_vertexLayout);
		m_vertices.resize(vertexCount*getVertexSizeInFloats());
		m_indices.resize(indexCount);
		for (UINT v = 0; v < vertexCount; v++)
		{
			if (VertexLayout::HasPositions(m_vertexLayout))
			{
				m_vertices[counter++] = vertices[v].position.x;
				m_vertices[counter++] = vertices[v].position.y;
				m_vertices[counter++] = vertices[v].position.z;
			}
			if (VertexLayout::HasTexcoords(m_vertexLayout))
			{
				m_vertices[counter++] = vertices[v].texcoord.x;
				m_vertices[counter++] = vertices[v].texcoord.y;
			}
			if (VertexLayout::HasNormals(m_vertexLayout))
			{
				m_vertices[counter++] = vertices[v].normal.x;
				m_vertices[counter++] = vertices[v].normal.y;
				m_vertices[counter++] = vertices[v].normal.z;
			}
			if (VertexLayout::HasTangentsBinormals(m_vertexLayout))
			{
				m_vertices[counter++] = vertices[v].tangent.x;
				m_vertices[counter++] = vertices[v].tangent.y;
				m_vertices[counter++] = vertices[v].tangent.z;
				m_vertices[counter++] = vertices[v].binormal.x;
				m_vertices[counter++] = vertices[v].binormal.y;
				m_vertices[counter++] = vertices[v].binormal.z;
			}
			if (VertexLayout::HasBones(m_vertexLayout))
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

	void ModelLoader::LoadAssimp(LPCWSTR filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(ToStr(filename).c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_SortByPType);
		if (scene == NULL)
		{
			//auto error = importer.GetErrorString();
			throw std::exception("Import failed");
		}
		StoreData(scene);
	}

#pragma region Load assimp

	void ModelLoader::StoreData(const aiScene *scene)
	{
		StoreMaterials(scene);
		StoreVertices(scene);
	}

	std::wstring FolderlessFilename(LPCSTR filename)
	{
		int lastSlashIndex = -1;
		for (int i = 0; filename[i]; i++)
			if (filename[i] == '\\' || filename[i] == '/')
				lastSlashIndex = i;
		std::wstring str;
		for (int i = lastSlashIndex + 1; filename[i]; i++)
			str += (WCHAR)filename[i];
		return str;
	}
	void ModelLoader::StoreMaterials(const aiScene *scene)
	{
		m_textureNames.resize(scene->mNumMaterials);
		m_normalmapNames.resize(scene->mNumMaterials);
		for (UINT i = 1; i < scene->mNumMaterials; i++)
		{
			if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				m_textureNames[i] = FolderlessFilename(path.data);
			}
			else
				m_textureNames[i] = L"";
			if (scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path);
				m_normalmapNames[i] = FolderlessFilename(path.data);
			}
			else
				m_normalmapNames[i] = L"";
		}
	}

	void ModelLoader::StoreVertices(const aiScene *scene)
	{
		m_groups.resize(scene->mNumMeshes);
		UINT vertexCount = 0;
		UINT indexCount = 0;
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh *mesh = scene->mMeshes[m];
			m_shaderType = ShaderType::ToShaderType(
				mesh->HasPositions(),
				mesh->HasTextureCoords(0) && m_textureNames[mesh->mMaterialIndex] != L"",
				mesh->HasNormals(),
				mesh->HasTextureCoords(0) && mesh->HasTangentsAndBitangents() && m_normalmapNames[mesh->mMaterialIndex] != L"",
				mesh->HasBones());
			m_vertexLayout = ShaderType::ToVertexLayout(m_shaderType);
			m_groups[m].startIndex = indexCount;
			m_groups[m].materialIndex = mesh->mMaterialIndex;
			vertexCount += scene->mMeshes[m]->mNumVertices;
			for (UINT ii = 0; ii < scene->mMeshes[m]->mNumFaces; ii++)
				indexCount += scene->mMeshes[m]->mFaces[ii].mNumIndices;
			m_groups[m].indexCount = indexCount - m_groups[m].startIndex;
		}
		m_vertexSizeInBytes = VertexLayout::VertexSizeInBytes(m_vertexLayout);
		m_vertices.resize(vertexCount*m_vertexSizeInBytes / sizeof(VertexElement));
		m_indices.resize(indexCount);

		vertexCount = 0;
		UINT vertexCounter = 0;
		UINT indexCounter = 0;
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh *mesh = scene->mMeshes[m];

			for (UINT v = 0; v < mesh->mNumVertices; v++)
			{
				if (VertexLayout::HasPositions(m_vertexLayout))
				{
					m_vertices[vertexCounter++] = mesh->mVertices[v].x;
					m_vertices[vertexCounter++] = mesh->mVertices[v].y;
					m_vertices[vertexCounter++] = -mesh->mVertices[v].z;
				}
				if (VertexLayout::HasTexcoords(m_vertexLayout))
				{
					m_vertices[vertexCounter++] = mesh->mTextureCoords[0][v].x;
					m_vertices[vertexCounter++] = 1.0f - mesh->mTextureCoords[0][v].y;
				}
				if (VertexLayout::HasNormals(m_vertexLayout))
				{
					m_vertices[vertexCounter++] = mesh->mNormals[v].x;
					m_vertices[vertexCounter++] = mesh->mNormals[v].y;
					m_vertices[vertexCounter++] = -mesh->mNormals[v].z;
				}
				if (VertexLayout::HasTangentsBinormals(m_vertexLayout))
				{
					m_vertices[vertexCounter++] = mesh->mTangents[v].x;
					m_vertices[vertexCounter++] = mesh->mTangents[v].y;
					m_vertices[vertexCounter++] = -mesh->mTangents[v].z;
					m_vertices[vertexCounter++] = -mesh->mBitangents[v].x;
					m_vertices[vertexCounter++] = -mesh->mBitangents[v].y;
					m_vertices[vertexCounter++] = mesh->mBitangents[v].z;
				}
				if (VertexLayout::HasBones(m_vertexLayout))
				{
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0u;
					m_vertices[vertexCounter++] = 0u;
					m_vertices[vertexCounter++] = 0u;
					m_vertices[vertexCounter++] = 0u;
				}
			}

			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				for (UINT i = 2; i < mesh->mFaces[f].mNumIndices; i++)
				{
					m_indices[indexCounter++] = mesh->mFaces[f].mIndices[0] + vertexCount;
					m_indices[indexCounter++] = mesh->mFaces[f].mIndices[i] + vertexCount;
					m_indices[indexCounter++] = mesh->mFaces[f].mIndices[i - 1] + vertexCount;
				}
			}
			vertexCount += mesh->mNumVertices;
		}
	}

#pragma endregion

	void ModelLoader::LoadOMD(LPCWSTR filename)
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
			LoadOMDText(filename);
		else if (filedata[0] == 'b' || filedata[0] == 'B')
			LoadOMDBinary(filename);
		else
			throw std::exception(std::string("Corrupted file: " + ToStr(filename)).c_str());
	}

#pragma region Load binary

	void ModelLoader::LoadOMDBinary(LPCWSTR filename)
	{
		std::ifstream infile(filename, std::ios::in | std::ios::binary);
		OMDHeader header;
		ReadHeaderBinary(infile, header);
		ReadVerticesBinary(infile, header);
		ReadIndicesBinary(infile, header);
		ReadGroupsBinary(infile, header);
		ReadMaterialsBinary(infile, header);
		ReadHitboxBinary(infile, header);
		ReadBonesBinary(infile, header);
		ReadAnimationsBinary(infile, header);
	}
	void ModelLoader::ReadHeaderBinary(std::ifstream& infile, OMDHeader& header)
	{
		infile.read((char*)&header, sizeof(header));
		m_vertexLayout = header.vertexLayout;
		m_vertexSizeInBytes = VertexLayout::VertexSizeInBytes(m_vertexLayout);
		m_shaderType = header.shaderType;
	}
	void ModelLoader::ReadVerticesBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_vertices.resize(header.vertexCount*m_vertexSizeInBytes / sizeof(VertexElement));
		infile.read((char*)m_vertices.data(), header.vertexCount*m_vertexSizeInBytes);
	}
	void ModelLoader::ReadIndicesBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_indices.resize(header.indexCount);
		infile.read((char*)m_indices.data(), header.indexCount * sizeof(UINT));
	}
	void ModelLoader::ReadGroupsBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_groups.resize(header.groupCount);
		infile.read((char*)m_groups.data(), header.groupCount * sizeof(VertexGroup));
	}
	void ModelLoader::ReadMaterialsBinary(std::ifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		m_textureNames.resize(header.materialCount);
		m_normalmapNames.resize(header.materialCount);
		for (UINT i = 0; i < header.materialCount; i++)
		{
			infile.read((char*)&ch, sizeof(WCHAR));
			while (ch)
			{
				m_textureNames[i] += ch;
				infile.read((char*)&ch, sizeof(WCHAR));
			}
			infile.read((char*)&ch, sizeof(WCHAR));
			while (ch)
			{
				m_normalmapNames[i] += ch;
				infile.read((char*)&ch, sizeof(WCHAR));
			}
		}
	}
	void ModelLoader::ReadHitboxBinary(std::ifstream& infile, OMDHeader& header)
	{
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			infile.read((char*)&m_bvPosition, sizeof(m_bvPosition));
			infile.read((char*)&m_bvCuboidSize, sizeof(m_bvCuboidSize));
			break;
		case mth::BoundingVolume::SPHERE:
			infile.read((char*)&m_bvPosition, sizeof(m_bvPosition));
			infile.read((char*)&m_bvSphereRadius, sizeof(m_bvSphereRadius));
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
	void ModelLoader::ReadBonesBinary(std::ifstream& infile, OMDHeader& header)
	{
	}
	void ModelLoader::ReadAnimationsBinary(std::ifstream& infile, OMDHeader& header)
	{
	}

#pragma endregion

#pragma region Load text

	void ModelLoader::LoadOMDText(LPCWSTR filename)
	{
		std::wifstream infile(filename, std::ios::in);
		OMDHeader header;
		ReadHeaderText(infile, header);
		ReadVerticesText(infile, header);
		ReadIndicesText(infile, header);
		ReadGroupsText(infile, header);
		ReadMaterialsText(infile, header);
		ReadHitboxText(infile, header);
		ReadBonesText(infile, header);
		ReadAnimationsText(infile, header);
	}
	void ModelLoader::ReadHeaderText(std::wifstream& infile, OMDHeader& header)
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
		infile >> header.vertexLayout;
		do { infile >> ch; } while (ch != ':');
		infile >> header.shaderType;
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
		m_vertexLayout = header.vertexLayout;
		m_vertexSizeInBytes = VertexLayout::VertexSizeInBytes(m_vertexLayout);
		m_shaderType = header.shaderType;
	}
	void ModelLoader::ReadVerticesText(std::wifstream& infile, OMDHeader& header)
	{
		bool positions = VertexLayout::HasPositions(header.vertexLayout);
		bool texcoords = VertexLayout::HasTexcoords(header.vertexLayout);
		bool normals = VertexLayout::HasNormals(header.vertexLayout);
		bool tgbinorm = VertexLayout::HasTangentsBinormals(header.vertexLayout);
		bool bones = VertexLayout::HasBones(header.vertexLayout);
		UINT posos = VertexLayout::PositionOffset(header.vertexLayout);
		UINT texos = VertexLayout::TexCoordOffset(header.vertexLayout);
		UINT normos = VertexLayout::NormalOffset(header.vertexLayout);
		UINT tgbnos = VertexLayout::TangentOffset(header.vertexLayout);
		UINT boneos = VertexLayout::BoneWeightsOffset(header.vertexLayout);
		UINT vertexSize = m_vertexSizeInBytes / sizeof(VertexElement);
		m_vertices.resize(header.vertexCount*vertexSize);
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		for (UINT i = 0; i < header.vertexCount; i++)
		{
			if (positions)
			{
				infile >> m_vertices[vertexSize*i + posos + 0].f;
				infile >> m_vertices[vertexSize*i + posos + 1].f;
				infile >> m_vertices[vertexSize*i + posos + 2].f;
			}
			if (texcoords)
			{
				infile >> m_vertices[vertexSize*i + texos + 0].f;
				infile >> m_vertices[vertexSize*i + texos + 1].f;
			}
			if (normals)
			{
				infile >> m_vertices[vertexSize*i + normos + 0].f;
				infile >> m_vertices[vertexSize*i + normos + 1].f;
				infile >> m_vertices[vertexSize*i + normos + 2].f;
			}
			if (tgbinorm)
			{
				infile >> m_vertices[vertexSize*i + tgbnos + 0].f;
				infile >> m_vertices[vertexSize*i + tgbnos + 1].f;
				infile >> m_vertices[vertexSize*i + tgbnos + 2].f;
				infile >> m_vertices[vertexSize*i + tgbnos + 3].f;
				infile >> m_vertices[vertexSize*i + tgbnos + 4].f;
				infile >> m_vertices[vertexSize*i + tgbnos + 5].f;
			}
			if (bones)
			{
				infile >> m_vertices[vertexSize*i + boneos + 0].f;
				infile >> m_vertices[vertexSize*i + boneos + 1].f;
				infile >> m_vertices[vertexSize*i + boneos + 2].f;
				infile >> m_vertices[vertexSize*i + boneos + 3].f;
				infile >> m_vertices[vertexSize*i + boneos + 4].u;
				infile >> m_vertices[vertexSize*i + boneos + 5].u;
				infile >> m_vertices[vertexSize*i + boneos + 6].u;
				infile >> m_vertices[vertexSize*i + boneos + 7].u;
			}
		}
	}
	void ModelLoader::ReadIndicesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_indices.resize(header.indexCount);
		for (UINT i = 0; i < header.indexCount; i++)
			infile >> m_indices[i];
	}
	void ModelLoader::ReadGroupsText(std::wifstream& infile, OMDHeader& header)
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
	void ModelLoader::ReadMaterialsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_textureNames.resize(header.materialCount);
		m_normalmapNames.resize(header.materialCount);
		for (UINT i = 0; i < header.materialCount; i++)
		{
			do { infile >> ch; } while (ch != ':');
			do { infile.read(&ch, 1); } while (ch == ' ');
			for (; ch != '\n'; infile.read(&ch, 1))
				m_textureNames[i] += ch;
			do { infile >> ch; } while (ch != ':');
			do { infile.read(&ch, 1); } while (ch == ' ');
			for (; ch != '\n'; infile.read(&ch, 1))
				m_normalmapNames[i] += ch;
		}
	}
	void ModelLoader::ReadHitboxText(std::wifstream& infile, OMDHeader& header)
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
	void ModelLoader::ReadBonesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
	}
	void ModelLoader::ReadAnimationsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
	}

#pragma endregion

#pragma region Export binary

	void ModelLoader::ExportOMDBinary(UINT shaderType, LPCWSTR filename)
	{
		OMDHeader header;
		header.fileFormat = 'B';
		header.extension[0] = 'O';
		header.extension[1] = 'M';
		header.extension[2] = 'D';
		header.vertexLayout = ShaderType::ToVertexLayout(shaderType);
		header.shaderType = shaderType;
		header.vertexCount = (UINT)m_vertices.size() / (m_vertexSizeInBytes / sizeof(VertexElement));
		header.indexCount = (UINT)m_indices.size();
		header.groupCount = (UINT)m_groups.size();
		header.materialCount = (UINT)m_textureNames.size();
		header.boundingVolumePrimitive = m_boundingVolumeType;
		header.hitboxTriangleCount = (UINT)m_hitbox.size();
		header.boneCount = 0;
		header.animationCount = 0;

		std::ofstream outfile(filename, std::ios::out | std::ios::binary);
		WriteHeaderBinary(outfile, header);
		WriteVerticesBinary(outfile, header);
		WriteIndicesBinary(outfile, header);
		WriteGroupsBinary(outfile, header);
		WriteMaterialsBinary(outfile, header);
		WriteHitboxBinary(outfile, header);
		WriteBonesBinary(outfile, header);
		WriteAnimationsBinary(outfile, header);
		outfile.close();
	}
	void ModelLoader::WriteHeaderBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)&header, sizeof(header));
	}
	void ModelLoader::WriteVerticesBinary(std::ofstream& outfile, OMDHeader& header)
	{
		bool positions = VertexLayout::HasPositions(header.vertexLayout);
		bool texcoords = VertexLayout::HasTexcoords(header.vertexLayout);
		bool normals = VertexLayout::HasNormals(header.vertexLayout);
		bool tgbinorm = VertexLayout::HasTangentsBinormals(header.vertexLayout);
		bool bones = VertexLayout::HasBones(header.vertexLayout);
		UINT posos = VertexLayout::PositionOffset(m_vertexLayout);
		UINT texos = VertexLayout::TexCoordOffset(m_vertexLayout);
		UINT normos = VertexLayout::NormalOffset(m_vertexLayout);
		UINT tgbnos = VertexLayout::TangentOffset(m_vertexLayout);
		UINT boneos = VertexLayout::BoneWeightsOffset(m_vertexLayout);
		UINT vertexSize = m_vertexSizeInBytes / sizeof(VertexElement);
		for (UINT i = 0; i < header.vertexCount; i++)
		{
			if (positions)
				outfile.write((char*)&m_vertices[vertexSize*i + posos], sizeof(VertexElement) * 3);
			if (texcoords)
				outfile.write((char*)&m_vertices[vertexSize*i + texos], sizeof(VertexElement) * 2);
			if (normals)
				outfile.write((char*)&m_vertices[vertexSize*i + normos], sizeof(VertexElement) * 3);
			if (tgbinorm)
				outfile.write((char*)&m_vertices[vertexSize*i + tgbnos], sizeof(VertexElement) * 6);
			if (bones)
				outfile.write((char*)&m_vertices[vertexSize*i + boneos], sizeof(VertexElement) * 8);
		}
	}
	void ModelLoader::WriteIndicesBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)m_indices.data(), sizeof(UINT) * m_indices.size());
	}
	void ModelLoader::WriteGroupsBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)m_groups.data(), m_groups.size() * sizeof(VertexGroup));
	}
	void ModelLoader::WriteMaterialsBinary(std::ofstream& outfile, OMDHeader& header)
	{
		for (UINT i = 0; i < header.materialCount; i++)
		{
			WCHAR nul = 0;
			if (ShaderType::HasTexture(header.shaderType))
				outfile.write((char*)m_textureNames[i].c_str(), (m_textureNames[i].length() + 1) * sizeof(WCHAR));
			else
				outfile.write((char*)&nul, sizeof(WCHAR));
			if (ShaderType::HasNormalmap(header.shaderType))
				outfile.write((char*)m_normalmapNames[i].c_str(), (m_normalmapNames[i].length() + 1) * sizeof(WCHAR));
			else
				outfile.write((char*)&nul, sizeof(WCHAR));
		}
	}
	void ModelLoader::WriteHitboxBinary(std::ofstream& outfile, OMDHeader& header)
	{
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			outfile.write((char*)&m_bvPosition, sizeof(m_bvPosition));
			outfile.write((char*)&m_bvCuboidSize, sizeof(m_bvCuboidSize));
			break;
		case mth::BoundingVolume::SPHERE:
			outfile.write((char*)&m_bvPosition, sizeof(m_bvPosition));
			outfile.write((char*)&m_bvSphereRadius, sizeof(m_bvSphereRadius));
			break;
		}
		if (header.hitboxTriangleCount)
			outfile.write((char*)m_hitbox.data(), header.hitboxTriangleCount * sizeof(mth::Triangle));
	}
	void ModelLoader::WriteBonesBinary(std::ofstream& outfile, OMDHeader& header)
	{
	}
	void ModelLoader::WriteAnimationsBinary(std::ofstream& outfile, OMDHeader& header)
	{
	}

#pragma endregion

#pragma region Export text

	void ModelLoader::ExportOMDText(UINT shaderType, LPCWSTR filename)
	{
		OMDHeader header;
		header.fileFormat = 'T';
		header.extension[0] = 'O';
		header.extension[1] = 'M';
		header.extension[2] = 'D';
		header.vertexLayout = ShaderType::ToVertexLayout(shaderType);
		header.shaderType = shaderType;
		header.vertexCount = (UINT)m_vertices.size() / (m_vertexSizeInBytes / sizeof(VertexElement));
		header.indexCount = (UINT)m_indices.size();
		header.groupCount = (UINT)m_groups.size();
		header.materialCount = (UINT)m_textureNames.size();
		header.boundingVolumePrimitive = 0;
		header.hitboxTriangleCount = 0;
		header.boneCount = 0;
		header.animationCount = 0;

		std::wofstream outfile(filename, std::ios::out);
		WriteHeaderText(outfile, header);
		WriteVerticesText(outfile, header);
		WriteIndicesText(outfile, header);
		WriteGroupsText(outfile, header);
		WriteMaterialsText(outfile, header);
		WriteHitboxText(outfile, header);
		WriteBonesText(outfile, header);
		WriteAnimationsText(outfile, header);
		outfile.close();
	}
	void ModelLoader::WriteHeaderText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << header.fileFormat;
		outfile << header.extension[0];
		outfile << header.extension[1];
		outfile << header.extension[2];
		outfile << std::endl;
		outfile << L"Vertex layout: " << header.vertexLayout << std::endl;
		outfile << L"Shader type: " << header.shaderType << std::endl;
		outfile << L"Vertex count: " << header.vertexCount << std::endl;
		outfile << L"Index count: " << header.indexCount << std::endl;
		outfile << L"Group count: " << header.groupCount << std::endl;
		outfile << L"Material count:" << header.materialCount << std::endl;
		outfile << L"Bounding volume primitive:" << header.boundingVolumePrimitive << std::endl;
		outfile << L"Hitbox triangle count:" << header.hitboxTriangleCount << std::endl;
		outfile << L"Bone count: " << header.boneCount << std::endl;
		outfile << L"Animation count: " << header.animationCount << std::endl;
	}
	void ModelLoader::WriteVerticesText(std::wofstream& outfile, OMDHeader& header)
	{
		bool positions = VertexLayout::HasPositions(header.vertexLayout);
		bool texcoords = VertexLayout::HasTexcoords(header.vertexLayout);
		bool normals = VertexLayout::HasNormals(header.vertexLayout);
		bool tgbinorm = VertexLayout::HasTangentsBinormals(header.vertexLayout);
		bool bones = VertexLayout::HasBones(header.vertexLayout);
		UINT posos = VertexLayout::PositionOffset(m_vertexLayout);
		UINT texos = VertexLayout::TexCoordOffset(m_vertexLayout);
		UINT normos = VertexLayout::NormalOffset(m_vertexLayout);
		UINT tgbnos = VertexLayout::TangentOffset(m_vertexLayout);
		UINT boneos = VertexLayout::BoneWeightsOffset(m_vertexLayout);
		UINT vertexSize = m_vertexSizeInBytes / sizeof(VertexElement);
		outfile << std::endl << L"Vertices:" << std::endl;
		for (UINT i = 0; i < header.vertexCount; i++)
		{
			if (positions)
			{
				outfile << m_vertices[vertexSize*i + posos + 0].f << ' ';
				outfile << m_vertices[vertexSize*i + posos + 1].f << ' ';
				outfile << m_vertices[vertexSize*i + posos + 2].f << ' ';
			}
			if (texcoords)
			{
				outfile << m_vertices[vertexSize*i + texos + 0].f << ' ';
				outfile << m_vertices[vertexSize*i + texos + 1].f << ' ';
			}
			if (normals)
			{
				outfile << m_vertices[vertexSize*i + normos + 0].f << ' ';
				outfile << m_vertices[vertexSize*i + normos + 1].f << ' ';
				outfile << m_vertices[vertexSize*i + normos + 2].f << ' ';
			}
			if (tgbinorm)
			{
				outfile << m_vertices[vertexSize*i + tgbnos + 0].f << ' ';
				outfile << m_vertices[vertexSize*i + tgbnos + 1].f << ' ';
				outfile << m_vertices[vertexSize*i + tgbnos + 2].f << ' ';
				outfile << m_vertices[vertexSize*i + tgbnos + 3].f << ' ';
				outfile << m_vertices[vertexSize*i + tgbnos + 4].f << ' ';
				outfile << m_vertices[vertexSize*i + tgbnos + 5].f << ' ';
			}
			if (bones)
			{
				outfile << m_vertices[vertexSize*i + boneos + 0].f << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 1].f << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 2].f << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 3].f << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 4].u << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 5].u << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 6].u << ' ';
				outfile << m_vertices[vertexSize*i + boneos + 7].u << ' ';
			}
			outfile << std::endl;
		}
	}
	void ModelLoader::WriteIndicesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Indices:" << std::endl;
		for (UINT i = 0; i < header.indexCount; i++)
			outfile << m_indices[i] << ' ';
		outfile << std::endl;
	}
	void ModelLoader::WriteGroupsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Groups:" << std::endl;
		for (UINT i = 0; i < header.groupCount; i++)
		{
			outfile << L"New group" << std::endl;
			outfile << L"\tStart index: " << m_groups[i].startIndex << std::endl;
			outfile << L"\tIndex count: " << m_groups[i].indexCount << std::endl;
			outfile << L"\tMaterial index: " << m_groups[i].materialIndex << std::endl;
		}
	}
	void ModelLoader::WriteMaterialsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Materials:" << std::endl;
		for (UINT i = 0; i < header.materialCount; i++)
		{
			outfile << L"New material" << std::endl;
			outfile << L"\tTexture name: ";
			if (ShaderType::HasTexture(header.shaderType))
				outfile << m_textureNames[i];
			outfile << std::endl;
			outfile << L"\tNormalmap name: ";
			if (ShaderType::HasNormalmap(header.shaderType))
				outfile << m_normalmapNames[i];
			outfile << std::endl;
		}
	}
	void ModelLoader::WriteHitboxText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Bounding volume:" << std::endl;
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			outfile << m_bvPosition.x << ' ';
			outfile << m_bvPosition.y << ' ';
			outfile << m_bvPosition.z << ' ';
			outfile << m_bvCuboidSize.x << ' ';
			outfile << m_bvCuboidSize.y << ' ';
			outfile << m_bvCuboidSize.z << ' ';
			outfile << std::endl;
			break;
		case mth::BoundingVolume::SPHERE:
			outfile << m_bvPosition.x << ' ';
			outfile << m_bvPosition.y << ' ';
			outfile << m_bvPosition.z << ' ';
			outfile << m_bvSphereRadius << ' ';
			outfile << std::endl;
			break;
		default:
			header.boundingVolumePrimitive = mth::BoundingVolume::NO_TYPE;
			break;
		}

		outfile << L"Hitbox:" << std::endl;
		if (header.hitboxTriangleCount)
		{
			for (UINT i = 0; i < header.hitboxTriangleCount; i++)
			{
				outfile << m_hitbox[i].getVertex(0).x << ' ';
				outfile << m_hitbox[i].getVertex(0).y << ' ';
				outfile << m_hitbox[i].getVertex(0).z << ' ';
				outfile << m_hitbox[i].getVertex(1).x << ' ';
				outfile << m_hitbox[i].getVertex(1).y << ' ';
				outfile << m_hitbox[i].getVertex(1).z << ' ';
				outfile << m_hitbox[i].getVertex(2).x << ' ';
				outfile << m_hitbox[i].getVertex(2).y << ' ';
				outfile << m_hitbox[i].getVertex(2).z << ' ';
				outfile << m_hitbox[i].getPlainNormal().x << ' ';
				outfile << m_hitbox[i].getPlainNormal().y << ' ';
				outfile << m_hitbox[i].getPlainNormal().z << ' ';
				outfile << m_hitbox[i].getPlainDistance() << ' ';
				outfile << std::endl;
			}
		}
	}
	void ModelLoader::WriteBonesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Bones:" << std::endl;
	}
	void ModelLoader::WriteAnimationsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Animations:" << std::endl;
	}

#pragma endregion

	ModelLoader::ModelLoader() :m_vertexSizeInBytes(0), m_vertexLayout(0) {}
	ModelLoader::ModelLoader(LPCWSTR filename) : m_vertexSizeInBytes(0), m_vertexLayout(0)
	{
		LoadModel(filename);
	}
	void ModelLoader::Clear()
	{
		m_folder.clear();
		m_filename.clear();
		m_vertexSizeInBytes = 0;
		m_vertexLayout = 0;
		m_shaderType = 0;
		m_boundingVolumeType = mth::BoundingVolume::NO_TYPE;
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
	void ModelLoader::ExportOMD(UINT shaderLayout, LPCWSTR filename, bool binary)
	{
		if (binary)
			ExportOMDBinary(shaderLayout, filename);
		else
			ExportOMDText(shaderLayout, filename);
	}
	void ModelLoader::LoadModel(LPCWSTR filename)
	{
		UINT lastSlashIndex = 0;
		UINT lastDotIndex = 0;
		UINT i;
		for (i = 0; filename[i]; i++)
		{
			if (filename[i] == '/' || filename[i] == '\\')
				lastSlashIndex = i;
			if (filename[i] == '.')
				lastDotIndex = i;
		}
		for (i = 0; i <= lastSlashIndex; i++)
			m_folder += filename[i];
		while (i < lastDotIndex)
			m_filename += filename[i++];
		if (filename[i + 0] == '.' &&
			filename[i + 1] == 'o' &&
			filename[i + 2] == 'm' &&
			filename[i + 3] == 'd'&&
			filename[i + 4] == '\0')
			LoadOMD(filename);
		else
			LoadAssimp(filename);
	}

	void ModelLoader::CreateCube(mth::float3 p, mth::float3 s, UINT shaderType)
	{
		Vertex_PTMB vertices[] = {
			{{p.x + s.x, p.y, p.z + s.z}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y, p.z}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z}, {1.0f, 0.0f}, {0.0f, -1.0f,0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z + s.z}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z}, { 0.0f, 1.0f} ,{ 0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x, p.y + s.y, p.z + s.z}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x, p.y + s.y, p.z},{ 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z + s.z}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z},{ 1.0f, 0.0f},{ 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z}, { 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z}, { 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f},{ 0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y + s.y, p.z}, { 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y + s.y, p.z}, { 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z}, { 0.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z}, { 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{p.x, p.y + s.y, p.z}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, 1.0f}},
			{{p.x, p.y + s.y, p.z + s.z}, { 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{p.x, p.y, p.z + s.z}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z}, { 0.0f, 1.0f},{ 0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y, p.z + s.z}, { 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z + s.z}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f}},
			{{p.x, p.y + s.y, p.z + s.z}, { 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}
		};
		UINT indices[] = {
			0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6, 8, 10, 9, 8, 11, 10, 12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22
		};
		Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), shaderType);
	}

	void ModelLoader::CreateFullScreenQuad()
	{
		CreateQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ShaderType::PT);
		Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f)*mth::float4x4::RotationX(-mth::pi*0.5f));
	}

	void ModelLoader::CreateScreenQuad(mth::float2 pos, mth::float2 size)
	{
		CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ShaderType::PT);
		Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f)*mth::float4x4::RotationX(-mth::pi*0.5f));
	}

	void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, UINT shaderType)
	{
		CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, shaderType);
	}

	void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT shaderType)
	{
		Vertex_PTMB vertices[] = {
		{ {pos.x + size.x, 0.0f, pos.y}, { tpos.x + tsize.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {pos.x + size.x, 0.0f, pos.y + size.y}, { tpos.x + tsize.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {pos.x, 0.0f, pos.y + size.y}, { tpos.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {pos.x, 0.0f, pos.y}, { tpos.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
		};
		UINT indices[] = { 0, 2, 1, 0, 3, 2 };
		Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), shaderType);
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
		if (VertexLayout::HasNormals(m_vertexLayout))
		{
			UINT offset = VertexLayout::NormalOffset(m_vertexLayout);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				m_vertices[i*vertexSize + offset + 0].f *= -1.0f;
				m_vertices[i*vertexSize + offset + 1].f *= -1.0f;
				m_vertices[i*vertexSize + offset + 2].f *= -1.0f;
			}
		}
		if (VertexLayout::HasTangentsBinormals(m_vertexLayout))
		{
			UINT offset1 = VertexLayout::TangentOffset(m_vertexLayout);
			UINT offset2 = VertexLayout::BinormalOffset(m_vertexLayout);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				m_vertices[i*vertexSize + offset1 + 0].f *= -1.0f;
				m_vertices[i*vertexSize + offset1 + 1].f *= -1.0f;
				m_vertices[i*vertexSize + offset1 + 2].f *= -1.0f;
				m_vertices[i*vertexSize + offset2 + 0].f *= -1.0f;
				m_vertices[i*vertexSize + offset2 + 1].f *= -1.0f;
				m_vertices[i*vertexSize + offset2 + 2].f *= -1.0f;
			}
		}
	}

	void ModelLoader::Transform(mth::float4x4 transform)
	{
		UINT vertexSize = getVertexSizeInFloats();
		mth::float4 v;
		if (VertexLayout::HasPositions(m_vertexLayout))
		{
			UINT offset = VertexLayout::PositionOffset(m_vertexLayout);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i*vertexSize + offset + 0].f;
				v.y = m_vertices[i*vertexSize + offset + 1].f;
				v.z = m_vertices[i*vertexSize + offset + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i*vertexSize + offset + 0] = v.x;
				m_vertices[i*vertexSize + offset + 1] = v.y;
				m_vertices[i*vertexSize + offset + 2] = v.z;
			}
		}
		if (VertexLayout::HasNormals(m_vertexLayout))
		{
			UINT offset = VertexLayout::NormalOffset(m_vertexLayout);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i*vertexSize + offset + 0].f;
				v.y = m_vertices[i*vertexSize + offset + 1].f;
				v.z = m_vertices[i*vertexSize + offset + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i*vertexSize + offset + 0] = v.x;
				m_vertices[i*vertexSize + offset + 1] = v.y;
				m_vertices[i*vertexSize + offset + 2] = v.z;
			}
		}
		if (VertexLayout::HasTangentsBinormals(m_vertexLayout))
		{
			UINT offset1 = VertexLayout::TangentOffset(m_vertexLayout);
			UINT offset2 = VertexLayout::BinormalOffset(m_vertexLayout);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i*vertexSize + offset1 + 0].f;
				v.y = m_vertices[i*vertexSize + offset1 + 1].f;
				v.z = m_vertices[i*vertexSize + offset1 + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i*vertexSize + offset1 + 0] = v.x;
				m_vertices[i*vertexSize + offset1 + 1] = v.y;
				m_vertices[i*vertexSize + offset1 + 2] = v.z;

				v.x = m_vertices[i*vertexSize + offset2 + 0].f;
				v.y = m_vertices[i*vertexSize + offset2 + 1].f;
				v.z = m_vertices[i*vertexSize + offset2 + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i*vertexSize + offset2 + 0] = v.x;
				m_vertices[i*vertexSize + offset2 + 1] = v.y;
				m_vertices[i*vertexSize + offset2 + 2] = v.z;
			}
		}
	}
}