#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimploader.h"

#pragma comment (lib, "Code/assimp/lib/assimp.lib")

namespace gfx
{
	void AssimpLoader::LoadAssimp(LPCWSTR filename, UINT modelType)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(ToStr(filename).c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_SortByPType);
		if (scene == NULL)
		{
			auto error = importer.GetErrorString();
			throw std::exception(("Importing " + ToStr(filename) + " failed: " + error).c_str());
		}
		StoreData(scene, modelType);
	}

	void AssimpLoader::StoreData(const aiScene* scene, UINT modelType)
	{
		StoreMaterials(scene, modelType);
		StoreVertices(scene, modelType);
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
	void AssimpLoader::StoreMaterials(const aiScene* scene, UINT modelType)
	{
		m_textures.resize(scene->mNumMaterials);
		for (UINT i = 1; i < scene->mNumMaterials; i++)
		{
			if (ModelType::HasTexture(modelType) && scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				m_textures[i].filename = FolderlessFilename(path.data);
			}
			else
				m_textures[i].filename = L"";
		}

		m_normalmaps.resize(scene->mNumMaterials);
		for (UINT i = 1; i < scene->mNumMaterials; i++)
		{
			if (ModelType::HasNormalmap(modelType) && scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path);
				m_normalmaps[i].filename = FolderlessFilename(path.data);
			}
			else
				m_normalmaps[i].filename = L"";
		}
	}

	void AssimpLoader::StoreVertices(const aiScene* scene, UINT modelType)
	{
		m_groups.resize(scene->mNumMeshes);
		UINT vertexCount = 0;
		UINT indexCount = 0;
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];
			m_modelType |= ModelType::ToModelType(
				mesh->HasPositions(),
				mesh->HasTextureCoords(0),
				mesh->HasNormals(),
				mesh->HasTangentsAndBitangents(),
				mesh->HasBones(),
				scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) > 0,
				scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_NORMALS) > 0);
			m_groups[m].startIndex = indexCount;
			m_groups[m].materialIndex = mesh->mMaterialIndex;
			vertexCount += scene->mMeshes[m]->mNumVertices;
			for (UINT ii = 0; ii < scene->mMeshes[m]->mNumFaces; ii++)
				indexCount += scene->mMeshes[m]->mFaces[ii].mNumIndices;
			m_groups[m].indexCount = indexCount - m_groups[m].startIndex;
		}
		m_modelType = ModelType::RemoveUnnecessary(m_modelType & modelType);
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize(vertexCount * m_vertexSizeInBytes / sizeof(VertexElement));
		m_indices.resize(indexCount);

		vertexCount = 0;
		UINT vertexCounter = 0;
		UINT indexCounter = 0;
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];

			for (UINT v = 0; v < mesh->mNumVertices; v++)
			{
				if (ModelType::HasPositions(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mVertices[v].x;
					m_vertices[vertexCounter++] = mesh->mVertices[v].y;
					m_vertices[vertexCounter++] = -mesh->mVertices[v].z;
				}
				if (ModelType::HasTexcoords(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mTextureCoords[0][v].x;
					m_vertices[vertexCounter++] = 1.0f - mesh->mTextureCoords[0][v].y;
				}
				if (ModelType::HasNormals(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mNormals[v].x;
					m_vertices[vertexCounter++] = mesh->mNormals[v].y;
					m_vertices[vertexCounter++] = -mesh->mNormals[v].z;
				}
				if (ModelType::HasTangentsBinormals(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mTangents[v].x;
					m_vertices[vertexCounter++] = mesh->mTangents[v].y;
					m_vertices[vertexCounter++] = -mesh->mTangents[v].z;
					m_vertices[vertexCounter++] = -mesh->mBitangents[v].x;
					m_vertices[vertexCounter++] = -mesh->mBitangents[v].y;
					m_vertices[vertexCounter++] = mesh->mBitangents[v].z;
				}
				if (ModelType::HasBones(m_modelType))
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
}